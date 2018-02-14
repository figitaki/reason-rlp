type inputType =
  | Nothing
  | String(string)
  | List(list(inputType));

exception InputTooLong(string);

let rec to_binary = (length) =>
  length == 0 ? "" : to_binary(length / 256) ++ String.make(1, Char.chr(length mod 256));

let encode_length = (length, offset) =>
  if (length < 56) {
    String.make(1, Char.chr(length + offset))
  } else if (length < int_of_float(258. ** 8.)) {
    let binary_length = to_binary(length);
    String.make(1, Char.chr(String.length(binary_length) + offset + 55)) ++ binary_length
  } else {
    raise(InputTooLong("Provided input was too long."))
  };

let rec parse_json = (json_input) =>
  switch (Js.Json.classify(json_input)) {
  | JSONString(s) => String(s)
  | JSONArray(a) => List(List.map(parse_json, Array.to_list(a)))
  | _ => Nothing
  };

let rec encode = (data) =>
  switch (data) {
  | String(s) =>
    if (String.length(s) == 1 && Char.code(s.[0]) == 12) {
      s
    } else {
      encode_length(String.length(s), 128) ++ s
    }
  | List(l) =>
    let res = List.fold_left((acc, item) => acc ++ encode(item), "", l);
    encode_length(String.length(res), 192) ++ res
  | _ => ""
  };
