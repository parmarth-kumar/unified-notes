// Correctness checks for the actual generators inside sorting-visualizer.html.
// Run: node DSA/Sorting/verify_visualizer.cjs
const fs = require('fs');
const path = require('path');
const vm = require('vm');
const assert = require('assert');

const html = fs.readFileSync(path.join(__dirname, 'sorting-visualizer.html'), 'utf8');
const source = html.match(/<script>([\s\S]*?)<\/script>/)[1];
new vm.Script(source);
function between(begin, end) {
  const first = source.indexOf(begin), last = source.indexOf(end, first);
  assert(first >= 0 && last > first, `Missing source boundary: ${begin}`);
  return source.slice(first, last);
}
const functions = between('function generateBubbleTrace(', '// ================= PERSISTENT BAR DOM ENGINE');
const registry = between('const ALGORITHMS =', 'const PRESETS =');
const parser = between('function parseCustomArray(', '// Custom Input:');
const runtime = vm.runInNewContext(functions + '\n' + registry + '\n' + parser + `
({ ALGORITHMS, QUICK_VARIANTS, validateSortingInput, parseCustomArray,
   generateStableCountingTrace, generateQuickTrace })`);
assert.equal(Object.keys(runtime.ALGORITHMS).length, 11);

let seed = 20261014;
function next() { seed = (Math.imul(seed, 1664525) + 1013904223) >>> 0; return seed; }
const cases = [[], [42], [3, 1, 2], [1, 2, 3], [3, 2, 1], [7, 7, 7],
  [-3, 4, 0, -2, 5, -3, 2], [Number.MAX_SAFE_INTEGER - 2, Number.MAX_SAFE_INTEGER, Number.MAX_SAFE_INTEGER - 1],
  [-Number.MAX_SAFE_INTEGER, 0, Number.MAX_SAFE_INTEGER]];
for (let trial = 0; trial < 120; ++trial) cases.push(Array.from({ length: next() % 16 }, () => next() % 11 - 5));
const variants = Object.entries(runtime.ALGORITHMS).map(([key, config]) => ({
  name: key, kind: key, stable: config.stable, generate: config.generate
})).filter(entry => entry.kind !== 'bucket' && entry.kind !== 'dnf');
variants.push({ name: 'stable-counting', kind: 'counting', stable: true, generate: runtime.generateStableCountingTrace });
variants.push({ name: 'quick-hoare', kind: 'quick', stable: false, generate: input => runtime.generateQuickTrace(input, 'hoare') });
variants.push({ name: 'quick-three-way', kind: 'quick', stable: false, generate: input => runtime.generateQuickTrace(input, 'three-way') });

let outputChecks = 0, rejectedChecks = 0, snapshotChecks = 0;
function verify(input, entry) {
  const original = [...input];
  const steps = entry.generate(input);
  assert.deepStrictEqual(input, original, `${entry.name} changed the supplied input`);
  assert(steps.length > 0, `${entry.name} has no initial state`);
  const final = steps.at(-1);
  if (runtime.validateSortingInput(input, entry.kind)) {
    // Old frequency-counting has its own range-warning snapshot; every added
    // generator uses status=error. Neither may claim successful sorting.
    assert(final.status === 'error' || /too large/i.test(final.msg), `${entry.name} accepted an invalid domain`);
    assert(!final.msg.startsWith('SUCCESS'), `${entry.name} claims success on invalid input`);
    rejectedChecks++;
    return steps;
  }
  let comparisons = -1, movements = -1;
  for (const step of steps) {
    assert.equal(step.items.length, input.length, `${entry.name} lost an item in a snapshot`);
    assert.equal(new Set(Array.from(step.items, item => item.id)).size, input.length, `${entry.name} duplicated an ID`);
    const placed = Array.from(step.items).filter(item => !item.lifted);
    assert.equal(new Set(placed.map(item => item.slot)).size, placed.length, `${entry.name} overlapped visible bars`);
    for (const item of step.items) {
      const origin = Number(item.id.slice(3));
      assert.equal(item.val, input[origin], `${entry.name} changed a record's value`);
      assert(item.slot >= 0 && item.slot < input.length, `${entry.name} has an invalid slot`);
    }
    assert(step.comps >= comparisons, `${entry.name} comparison count moved backwards`);
    assert(step.swaps >= movements, `${entry.name} movement count moved backwards`);
    comparisons = step.comps; movements = step.swaps;
    snapshotChecks++;
  }
  const items = Array.from(final.items).sort((a, b) => a.slot - b.slot);
  assert.deepStrictEqual(items.map(item => item.val), [...input].sort((a, b) => a - b), `${entry.name} sorted incorrectly`);
  assert.deepStrictEqual(items.map(item => item.slot), Array.from({ length: input.length }, (_, index) => index));
  if (entry.stable) {
    const expected = input.map((val, origin) => ({ val, origin })).sort((a, b) => a.val - b.val || a.origin - b.origin);
    assert.deepStrictEqual(items.map(item => Number(item.id.slice(3))), expected.map(item => item.origin), `${entry.name} changed equal-key order`);
  }
  assert(final.highlights === 'all-sorted', `${entry.name} has no sorted final state`);
  outputChecks++;
  return steps;
}
for (const input of cases) for (const entry of variants) verify(input, entry);

const bucket = { name: 'bucket', kind: 'bucket', stable: true, generate: runtime.ALGORITHMS.bucket.generate };
const radix = { name: 'radix', kind: 'radix', stable: true, generate: runtime.ALGORITHMS.radix.generate };
for (const input of [[], [0.42], [0, 0.9999999999999999], [0.42, 0.12, 0.35, 0.18], [0.42, 0.42, 0.12, 0.12]]) verify(input, bucket);
for (let trial = 0; trial < 150; ++trial) {
  verify(Array.from({ length: next() % 16 }, () => (next() % 1000) / 1000), bucket);
  verify(Array.from({ length: next() % 16 }, () => next() % 2147483648), radix);
}
verify([9007199254740991, 0, 9007199254740990, 9007199254740991], radix);
const dnf = { name: 'dnf', kind: 'dnf', stable: false, generate: runtime.ALGORITHMS.dnf.generate };
let dnfCases = 0;
for (let n = 0; n <= 6; n++) {
  const count = 3 ** n;
  for (let mask = 0; mask < count; mask++) {
    let code = mask;
    const input = Array.from({ length: n }, () => { const value = code % 3; code = Math.floor(code / 3); return value; });
    const trace = verify(input, dnf);
    assert.equal(trace.filter(step => step.msg.startsWith('INSPECT')).length, n);
    assert.equal(trace.at(-1).i, input.filter(value => value !== 2).length, 'Final DNF mid is the start of the twos region');
    dnfCases++;
  }
}
for (const input of [[-1], [1], [Infinity], [NaN], [-0.1, 0.5], [0.5, 1]]) verify(input, bucket);
for (const input of [[-1], [1.5], [Infinity], [NaN]]) verify(input, radix);
for (const input of [[3], [-1], [0.5], [0, 1, 4]]) verify(input, dnf);

// Exact examples verify algorithm mechanics rather than just final values.
const bubble = runtime.ALGORITHMS.bubble.generate([3, 1, 2, 4, 5, 6]).at(-1);
assert.equal(bubble.comps, 9); assert.equal(bubble.swaps, 2);
const selection = runtime.ALGORITHMS.selection.generate([3, 1, 2, 4, 5, 6]).at(-1);
assert.equal(selection.comps, 15); assert.equal(selection.swaps, 2);
const insertion = runtime.ALGORITHMS.insertion.generate([3, 1, 2, 4, 5, 6]).at(-1);
assert.equal(insertion.comps, 6); assert.equal(insertion.swaps, 7);
const merge = runtime.ALGORITHMS.merge.generate([3, 1, 2]).at(-1);
assert.equal(merge.comps, 3); assert.equal(merge.swaps, 10);
const heap = runtime.ALGORITHMS.heap.generate([4, 1, 3, 2]).at(-1);
assert.equal(heap.comps, 6); assert.equal(heap.swaps, 6);
const lomuto = runtime.generateQuickTrace([3, 1, 2], 'lomuto').at(-1);
assert.equal(lomuto.comps, 2); assert.equal(lomuto.swaps, 2);
const hoare = runtime.generateQuickTrace([10, 80, 30, 90, 40, 50, 70], 'hoare');
assert(hoare.some(step => step.msg.startsWith('SPLIT: Return 5.')));
const equalThreeWay = runtime.generateQuickTrace(Array(8).fill(7), 'three-way').at(-1);
assert.equal(equalThreeWay.comps, 16); assert.equal(equalThreeWay.swaps, 0);
const shell = runtime.ALGORITHMS.shell.generate([8, 7, 6, 5, 4, 3, 2, 1]);
const gapFour = shell.find(step => step.msg === 'GAP COMPLETE: Each gap-4 group is sorted.');
assert(gapFour);
assert.deepStrictEqual(Array.from(gapFour.items).sort((a, b) => a.slot - b.slot).map(item => item.val), [4, 3, 2, 1, 8, 7, 6, 5]);
assert.equal(shell.at(-1).i, 1, 'Shell Sort finishes with gap 1');
const counted = runtime.generateStableCountingTrace([4, -1, 2, 4, -1]);
const lastCount = counted.at(-1);
assert.equal(lastCount.comps, 8); assert.equal(lastCount.swaps, 5);
assert.deepStrictEqual(Array.from(lastCount.buffer, item => item.id), ['el_1', 'el_4', 'el_2', 'el_0', 'el_3']);
const digitPass = runtime.ALGORITHMS.radix.generate([21, 13, 12]);
const ones = digitPass.find(step => step.msg.startsWith('PASS COMPLETE'));
assert.deepStrictEqual(Array.from(ones.items).sort((a, b) => a.slot - b.slot).map(item => item.val), [21, 12, 13]);

const parse = runtime.parseCustomArray;
assert.deepStrictEqual(Array.from(parse('[]')), []);
assert.deepStrictEqual(Array.from(parse(' 3, -2, +1, 0 ')), [3, -2, 1, 0]);
assert.deepStrictEqual(Array.from(parse('0.42, .12, 0, 0.999', 'bucket')), [0.42, 0.12, 0, 0.999]);
for (const invalid of ['', '3, nope, 2', '1.5,2', '1e3,2', '3,,2', '3,', '9007199254740992', Array(16).fill(1).join(',')]) assert.throws(() => parse(invalid));
for (const invalid of ['NaN', 'Infinity', '0.5,,0.2', '.5junk', '1e-2', '0.2,']) assert.throws(() => parse(invalid, 'bucket'));

// The UI's generated links and the configuration must agree with real notes.
const theory = fs.readFileSync(path.join(__dirname, 'theory.md'), 'utf8');
for (const [key, config] of Object.entries(runtime.ALGORITHMS)) {
  assert(html.includes(`data-algo="${key}"`), `No control for ${key}`);
  assert(theory.includes(`id="${config.anchor}"`), `Missing lesson anchor for ${key}`);
}
assert(html.includes('@media (prefers-reduced-motion: reduce)'));
assert(!/transition:\s*left/.test(html));
console.log(`PASS: ${outputChecks} valid generator cases, ${rejectedChecks} domain rejections, ${snapshotChecks} identity/counter snapshots, ${dnfCases} exhaustive DNF cases; 11 algorithms and all 14 variants`);
console.log('PASS: exact partition/merge/heap/gap/digit mechanics, stable output IDs, parser rules, generated lesson anchors, JavaScript syntax, and reduced-motion/transform rules');
