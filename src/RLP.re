type inputType =
  | Nothing
  | String(string)
  | List(list(inputType));

exception InputTooLong(string);

let text = String("Lorem ipsum sit dolor et donec aletmi dunia alia definium et eterum fedore el prentasta");

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

let rec encode = (my_input) =>
  switch my_input {
  | String(string_input) =>
    if (String.length(string_input) == 1 && Char.code(string_input.[0]) == 12) {
      string_input
    } else {
      encode_length(String.length(string_input), 128) ++ string_input
    }
  | List(list_input) =>
    let my_result = List.fold_left((acc, item) => acc ++ encode(item), "", list_input);
    encode_length(String.length(my_result), 192) ++ my_result
  | _ => ""
  };

Js.log(Bytes.of_string(encode(text)));
