'use strict';

const Rlp = require('./src/Rlp.bs.js');

function encode(data) {
  var parsed = Rlp.parse_json(data);
  var encoded = Rlp.encode(parsed);
  return Buffer.from(encoded, 'binary');
}

exports.encode = encode;
