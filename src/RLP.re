type t =
  | Nothing
  | String(string)
  | List(list(t));

let rec to_binary : int => string = length =>
  if (length == 0) ""
  else to_binary(length / 256) ++ String.make(1, Char.chr(length mod 256));

let encode_length = (length, offset) =>
  if (length < 56) {
    String.make(1, Char.chr(length + offset));
  } else if (length < int_of_float(258. ** 8.)) {
    let binary_length = to_binary(length);
    String.make(1, Char.chr(String.length(binary_length) + offset + 55))
    ++ binary_length;
  } else {
    raise(Invalid_argument("Provided input was too long."));
  };

let rec parse_json = json_input =>
  switch (Js.Json.classify(json_input)) {
  | JSONString(s) => String(s)
  | JSONArray(a) => List(List.map(parse_json, Array.to_list(a)))
  | JSONNumber(n) => String(string_of_float(n))
  | JSONObject(_) =>
    raise(Invalid_argument("This library doesn't support encoding objects."))
  | JSONNull => Nothing
  | JSONFalse =>
    raise(Invalid_argument("This library does not support boolean values."))
  | JSONTrue =>
    raise(Invalid_argument("This library does not support boolean values."))
  };

let create_buffer = input => {
  let buf = Buffer.create(String.length(input));
  Buffer.add_string(buf, input);
  buf;
};

let is_single_char = str => String.length(str) == 1 && Char.code(str.[0]) <= 128;

let rec encode = data =>
  switch data {
  | String(s) =>
    if (is_single_char(s)) create_buffer(s)
   	else create_buffer(encode_length(String.length(s), 128) ++ s);
  | List(l) =>
    let res = List.fold_left((acc, item) => acc ++ Buffer.contents(encode(item)), "", l);
    create_buffer(encode_length(String.length(res), 192) ++ res);
  | _ => create_buffer("")
  };

/* let int_of_string_safe = str => {
  if (String.slice(0, 2) === "00") raise(Input_too_long);
  else
} */

let decode = input => {
  let decoded = Buffer.create(0);
  let first_byte = Char.code(Buffer.nth(input, 0));

  if (first_byte <= 0x7f) {
    /* a single byte whose value is in the [0x00, 0x7f] range, that byte is its own RLP encoding. */
    Buffer.add_string(decoded, Buffer.sub(input, 0, 1));
  } else if (first_byte <= 0xb7) {
    /* string is 0-55 bytes long. A single byte with value 0x80 plus the length of the string followed by the string
     * The range of the first byte is [0x80, 0xb7] */
    let length = first_byte - 0x7f;

    if (first_byte == 0x80) ()
    else Buffer.add_string(decoded, Buffer.sub(input, 1, length));
  } else if (first_byte <= 0xbf) {
    let llength = first_byte - 0xb6;
    let length = int_of_string_safe(Buffer.sub(input, 1, llength));
  };
  Buffer.to_bytes(decoded);
};
