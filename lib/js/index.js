'use strict';

const Rlp = require('./src/Rlp.bs.js');

function encode(data) {
  var parsed = Rlp.parse_json(data);
  var encoded = Rlp.encode(parsed);
  return Buffer.from(encoded, 'binary');
}

function decode(data) {
  if (Buffer.isBuffer(data)) data = data.toString()
  var input_buffer = Rlp.create_buffer(data);
  var decoded = Rlp.decode(input_buffer);
  return Buffer.from(decoded);
}

exports.encode = encode;
exports.decode = decode;
