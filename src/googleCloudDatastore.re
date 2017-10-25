type key_;

type kind 'a = (string, 'a);

type path 'a = Js.t {. namespace : string, path : kind 'a};

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
  external get : t => key_ => (Js.nullable string => array (Js.t {..}) => unit) =>
    unit = "get" [@@bs.send];
  external getMultiple :
    t => array key_ => (Js.nullable string => array (Js.t {..}) => unit) => unit =
    "get" [@@bs.send];

  /** [save datastore] Insert or update the specified object(s). If a key is
      incomplete, its associated object is inserted and the original Key object
      is updated to contain the generated ID. **/
  external save : t => Js.t {. key : key_, data : Js.t {..}} =>
    (Js.nullable string => Js.Json.t => unit) => unit = "save" [@@bs.send];
  external saveMultiple : t => array (Js.t {. key : key_, data : Js.t {..}}) =>
    (Js.nullable string => Js.Json.t => unit) => unit = "save" [@@bs.send];
};
