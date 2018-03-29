# Recursive Length Prefix Encoding
[![Travis](https://img.shields.io/travis/figitaki/reason-rlp.svg?style=flat-square)](https://travis-ci.org/figitaki/reason-rlp)
[![npm version](https://img.shields.io/npm/v/reason-rlp.svg?style=flat-square)](https://www.npmjs.com/package/reason-rlp)
[![code style: prettier](https://img.shields.io/badge/code_style-prettier-ff69b4.svg?style=flat-square)](https://github.com/prettier/prettier)

[Recursive Length](https://github.com/ethereum/wiki/wiki/RLP) prefix encoding implementation in Reason.

## Install

`npm install reason-rlp`

Install with `-g` if you want to use the cli.

## Usage

```js
var RLP = require('reason-rlp');
var assert = require('assert');

var nestedList = [ [], [[]], [ [], [[]] ] ]; /* Set theoretic representation of 3 */
var encoded = RLP.encode(nestedList);
var decoded = RLP.decode(encoded);
assert.deepEquals(nestedList, decoded);
```

## API

`rlp.encode(plain)` - RLP encoded an `Array`, `Buffer` or `String` and returns a `Buffer`
**WIP** `rlp.decode(encoded, [skipRemainderCheck=false])` - Decodes an RLP encoded `Buffer`, `Array`
or `String` and returns a `Buffer` or an `Array` of `Buffer`s. If `skipRemainderCheck` is enabled
`rlp` will just decode the first rlp sequence in the buffer. By default it would through an error
if there is more bytes in Buffer than used by the rlp sequence.

# Tests
Tests use mocha. To run `npm test`
