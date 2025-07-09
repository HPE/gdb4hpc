use std::time::Duration;
use std::collections::HashMap;

struct Move {
    i: usize,
    j: usize,
    dir: usize,
}

struct User {
    active: bool,
    moves: Vec<Move>,
    ip: IpAddr,
    username: String,
    email: String,
    sign_in_count: u64,
}
struct UserNoStrings {
    active: bool,
    sign_in_count: u64,
}
enum IpAddrKind {
    V4,
    V6,
}
struct IpAddr {
    kind: IpAddrKind,
    address: String,
}


fn main() {
    let mut book_reviews: HashMap<String, String> = HashMap::new();

    book_reviews.insert(
        "Adventures of Huckleberry Finn".to_string(),
        "My favorite book.".to_string(),
    );
    book_reviews.insert(
        "Grimms' Fairy Tales".to_string(),
        "Masterpiece.".to_string(),
    );
    let mut _user1 = User {
        active: true,
        username: String::from("someusername123"),
        moves: Vec::new(),
        email: String::from("someone@example.com"),
        sign_in_count: 1,
        ip: IpAddr {
            kind: IpAddrKind::V4,
            address: String::from("127.0.0.1"),
        },
    };
    _user1.moves.push(Move{ i: 2, j: 3, dir: 4});
    _user1.moves.push(Move{ i: 1, j: 2, dir: 3});

    let _user2 = UserNoStrings {
        active: true,
        sign_in_count: 10, 
    };

    let _four = IpAddrKind::V4;
    let _six = IpAddrKind::V6;

    
    let home = IpAddr {
        kind: IpAddrKind::V4,
        address: String::from("127.0.0.1"),
    };

    let _boolean: bool = true;
    let _float64: f64 = 1.23456789;
    let _float32: f32 = -9.87654;
    let _archisize: isize = -1000;
    let _archusize: usize = 1000;
    // does not currently work
    let _sinteger128: i128 = -128;
    let _sinteger64: i64 = -64;
    let _sinteger32: i32 = -32;
    let mut _sinteger16: i16 = -16;
    let _sinteger8: i8 = -8;
    // does not currently work
    let _uinteger128: u128 = 128;
    let _uinteger64: u64 = 64;
    let _uinteger32: u32 = 32;
    let _uinteger16: u16 = 16;
    let _uinteger8: u8 = 8;
    let _charsafe: char = 'z';
    // does not currently work
    // expected (result in rust-gdb): $9 = 8484 '\u{002124}'
    // result in gdb4hpc: 36 '$' (looks like its interpreting the first hex "24").
    let _chariffy: char = 'ℤ';
    let _v = vec!['h', 'e', 'l', 'l', 'o'];
    let _vararr1 = vec![char::from_digit(1, 10).unwrap(), 'ȝ', '\'', '\n', '\t', 'ǿ', '⛞', '⛄'];
    let _vararr2 = [char::from_digit(1, 10).unwrap(), 'ȝ', '\'', '\n', '\t', 'ǿ', '⛞', '⛄'];
    // does not currently work
    let tup: (i32, f64, u8) = (500, 6.4, 1);
    // all three of these work. _vector.len returns 3. I am not sure why the vector one works tbh.
    let _five_hundred = tup.0;
    let _arr: [i32; 5] = [1, 2, 3, 4, 5];
    let _vector: Vec<i32> = vec![1,2,3];

    let pangram: &'static str = "the quick brown fox jumps over the lazy dog";
    let _pint = &mut _sinteger16;
    let my_num_raw: *const i64 = &_sinteger64;
    *_pint += 1;

    let chars: Vec<char> = pangram.chars().collect();
    let mut string = String::new();
    for c in chars {
        string.push(c);
        string.push_str(", ");
    }

    let _alice = String::from("Alℤiℤcℤe");

    // just adding this so when running locally gdb4hpc is able to connect.
    std::thread::sleep(Duration::from_millis(5000));
}

