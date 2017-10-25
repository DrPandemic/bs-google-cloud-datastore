open GoogleCloudDatastore;

let app = Datastore.make ();

let key0 = Datastore.keyByKind app "test";

let key1 = Datastore.keyByID app ("test", 42);

let key2 = Datastore.keyByName app ("test", "name");

let key3 = Datastore.keyByPathAndID app {"namespace": "wow", "path": ("test", 42)};

let key4 = Datastore.keyByPathAndName app {"namespace": "wow", "path": ("test12", "wow")};

type entry = Js.t {. name: string};

Datastore.save app {"key": key1, "data": { "name": "that name", "yes": "42" }} (fun a b => {
  Js.log a;
  Js.log b
});

Datastore.save app {"key": key2, "data": { "name": "second", "yes": 42 }} (fun a b => {
  Js.log a;
  Js.log b
});

Datastore.get app key1 (fun a b => {
  Js.log a;
  Js.log b
});

Datastore.getMultiple app [| key1, key2 |] (fun a b => {
  Js.log a;
  Js.log b
});

Datastore.delete app key1 (fun a b => {
  Js.log a;
  Js.log b
});
