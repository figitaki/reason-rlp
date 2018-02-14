// Generated by BUCKLESCRIPT VERSION 2.1.0, PLEASE EDIT WITH CARE
'use strict';

var Char            = require("bs-platform/lib/js/char.js");
var List            = require("bs-platform/lib/js/list.js");
var $$Array         = require("bs-platform/lib/js/array.js");
var Block           = require("bs-platform/lib/js/block.js");
var $$String        = require("bs-platform/lib/js/string.js");
var Js_json         = require("bs-platform/lib/js/js_json.js");
var Caml_string     = require("bs-platform/lib/js/caml_string.js");
var Caml_exceptions = require("bs-platform/lib/js/caml_exceptions.js");

var InputTooLong = Caml_exceptions.create("Rlp-RlpReason.InputTooLong");

function to_binary(length) {
  var match = +(length === 0);
  if (match !== 0) {
    return "";
  } else {
    return to_binary(length / 256 | 0) + $$String.make(1, Char.chr(length % 256));
  }
}

function encode_length(length, offset) {
  if (length < 56) {
    return $$String.make(1, Char.chr(length + offset | 0));
  } else if (length < (Math.pow(258, 8) | 0)) {
    var binary_length = to_binary(length);
    return $$String.make(1, Char.chr((binary_length.length + offset | 0) + 55 | 0)) + binary_length;
  } else {
    throw [
          InputTooLong,
          "Provided input was too long."
        ];
  }
}

function parse_json(json_input) {
  var match = Js_json.classify(json_input);
  if (typeof match === "number") {
    return /* Nothing */0;
  } else {
    switch (match.tag | 0) {
      case 0 : 
          return /* String */Block.__(0, [match[0]]);
      case 3 : 
          return /* List */Block.__(1, [List.map(parse_json, $$Array.to_list(match[0]))]);
      default:
        return /* Nothing */0;
    }
  }
}

function encode(data) {
  if (typeof data === "number") {
    return "";
  } else if (data.tag) {
    var res = List.fold_left((function (acc, item) {
            return acc + encode(item);
          }), "", data[0]);
    return encode_length(res.length, 192) + res;
  } else {
    var s = data[0];
    if (s.length === 1 && Caml_string.get(s, 0) === 12) {
      return s;
    } else {
      return encode_length(s.length, 128) + s;
    }
  }
}

exports.InputTooLong  = InputTooLong;
exports.to_binary     = to_binary;
exports.encode_length = encode_length;
exports.parse_json    = parse_json;
exports.encode        = encode;
/* No side effect */