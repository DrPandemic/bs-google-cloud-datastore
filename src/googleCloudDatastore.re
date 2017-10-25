type key_;

type kind 'a = (string, 'a);

type path 'a = Js.t {. namespace : string, path : kind 'a};

/** From bs-express */
module Error = {
  type t;
  /** Error type */

  external message : t => option string = ""
    [@@bs.send][@@bs.return null_undefined_to_opt];
  external name : t => option string = ""
    [@@bs.send][@@bs.return null_undefined_to_opt];
};

module Datastore = {
  type t;

  /** [make ()] creates an instance of the datastore class. */
  external make : unit => t = "@google-cloud/datastore" [@@bs.module];

  /** [key datastore] helpers to create a Key object **/
  external keyByKind : t => string => key_ = "key" [@@bs.send];
  external keyByID : t => (string, int) => key_ = "key" [@@bs.send];
  external keyByName : t => (string, string) => key_ = "key" [@@bs.send];
  external keyByPathAndID : t => path int => key_ = "key" [@@bs.send];
  external keyByPathAndName : t => path string => key_ = "key" [@@bs.send];

  /** [get datastore] retrieve the entities identified with the specified key(s)
      in the current transaction. **/
  external get : t => key_ => (Js.nullable Error.t => array (Js.t {..}) => unit) =>
    unit = "get" [@@bs.send];
  external getMultiple :
    t => array key_ => (Js.nullable Error.t => array (Js.t {..}) => unit) => unit =
    "get" [@@bs.send];

  /** [save datastore] Insert or update the specified object(s). If a key is
      incomplete, its associated object is inserted and the original Key object
      is updated to contain the generated ID. **/
  external save : t => Js.t {. key : key_, data : Js.t {..}} =>
    (Js.nullable Error.t => Js.Json.t => unit) => unit = "save" [@@bs.send];
  external saveMultiple : t => array (Js.t {. key : key_, data : Js.t {..}}) =>
    (Js.nullable Error.t => Js.Json.t => unit) => unit = "save" [@@bs.send];

  /** [delete datastore] Delete all entities identified with the specified
      key(s). **/
  external delete : t => key_ => (Js.nullable Error.t => Js.Json.t => unit) => unit =
    "delete" [@@bs.send];
  external deleteMultiple : t => array key_ =>
    (Js.nullable Error.t => Js.Json.t => unit) => unit = "delete" [@@bs.send];
};
