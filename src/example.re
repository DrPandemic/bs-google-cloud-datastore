open GoogleCloudDatastore;

let app = Datastore.make ();

let key0 = Datastore.keyByKind app "test";

let key1 = Datastore.keyByID app ("test", 42);

let key2 = Datastore.keyByName app ("test", "name");

let key3 = Datastore.keyByPath app {"namespace": "wow", "kind": ID ("test", 42)};
