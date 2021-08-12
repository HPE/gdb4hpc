@0xdbfde34651f4f8e3;

# this defines how messages are generated, serialized and deserialized on the
# gdb4hpc server side and client side. it is fed into the capnp compiler program,
# capnpc, to generate the c++ files needed by the source code.

struct ZmqnetMessagePackage {
    messages @0 :List(ZmqnetMessage);

    struct ZmqnetMessage {
        data @0 :Text;
        hash @1 :UInt64;
        bitset @2 :Data;
    }
}

struct InternalCommand {
    newStreamCmd :group {
        streamId @0 :Int32;
    }
}
