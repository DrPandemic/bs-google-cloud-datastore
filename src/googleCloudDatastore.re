type key_;

type kind =
  | ID (string, int)
  | Name (string, string);

type path = Js.t {. namespace : string, kind : kind};

module Datastore = {
  type t;

  /** [make ()] creates an instance of the datastore class. */
  external make : unit => t = "@google-cloud/datastore" [@@bs.module];

  /** [key datastore] helpers to create a Key object **/
  external keyByKind : t => string => key_ = "key" [@@bs.send];
  external keyByID : t => (string, int) => key_ = "key" [@@bs.send];
  external keyByName : t => (string, string) => key_ = "key" [@@bs.send];
  external keyByPath : t => path => key_ = "key" [@@bs.send];
};
