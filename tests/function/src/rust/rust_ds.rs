use std::collections::HashMap;

struct Move {
    _i: usize,
    _j: usize,
    _dir: usize,
}

struct User {
    _active: bool,
    moves: Vec<Move>,
    _ip: IpAddr,
    _username: String,
    _email: String,
    _sign_in_count: u64,
}
struct UserNoStrings {
    _active: bool,
    _sign_in_count: u64,
}
enum IpAddrKind {
    V4,
    V6,
}
struct IpAddr {
    _kind: IpAddrKind,
    _address: String,
}
struct Complex<'a> {
    _ipaddr: IpAddrKind,
    _name: String,
    _hobbies: Vec<String>,
    _favorite_numbers: Vec<u32>,
    _address: String,
    _static_state: &'a str,
    _string_country: String,
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
        _active: true,
        _username: String::from("someusername123"),
        moves: Vec::new(),
        _email: String::from("someone@example.com"),
        _sign_in_count: 1,
        _ip: IpAddr {
            _kind: IpAddrKind::V4,
            _address: String::from("127.0.0.1"),
        },
    };
    _user1.moves.push(Move{ _i: 2, _j: 3, _dir: 4});
    _user1.moves.push(Move{ _i: 1, _j: 2, _dir: 3});

    let _user2 = UserNoStrings {
        _active: true,
        _sign_in_count: 10, 
    };

    let _four = IpAddrKind::V4;
    let _six = IpAddrKind::V6;

    
    let _home = IpAddr {
        _kind: IpAddrKind::V4,
        _address: String::from("127.0.0.1"),
    };

    let _boolean: bool = true;
    let _float64: f64 = 1.23456789;
    let _float32: f32 = -9.87654;
    let _archisize: isize = -500000000000000000;
    let _archusize: usize = 9000000000000000000;
    // does not currently work
    let _sinteger128: i128 = -128;
    let _sinteger64: i64 = -9000000000000000000;
    let _sinteger32: i32 = -2000000000;
    let mut _sinteger16: i16 = -32000;
    let _sinteger8: i8 = -100;
    // does not currently work
    let _uinteger128: u128 = 200;
    let _uinteger64: u64 = 18000000000000000000;
    let _uinteger32: u32 = 4000000000;
    let _uinteger16: u16 = 65000;
    let _uinteger8: u8 = 200;
    let _char_safe: char = 'z';
    let _char_unicode: char = 'ℤ';
    let _vec_ascii = vec!['h', 'e', 'l', 'l', 'o'];
    let _vec_complex = vec![char::from_digit(1, 10).unwrap(), '?', '\'', '\n', '\t', '?', '\n', '⛄'];
    let _arr_complex = [char::from_digit(1, 10).unwrap(), 'ȝ', '\'', '\n', '\t', 'ǿ', '⛞', '⛄'];
    // does not currently work
    let tup: (i32, f64, u8) = (500, 6.4, 1);
    // all three of these work. _vector.len returns 3. 
    let _tup_object_i32 = tup.0;

    let pangram: &'static str = "the quick brown fox jumps over the lazy dog";
    let _pint = &mut _sinteger16;
    let _my_num_raw: *const i64 = &_sinteger64;
    *_pint += 1;

    let chars: Vec<char> = pangram.chars().collect();
    let mut string = String::new();
    for c in chars {
        string.push(c);
        string.push_str(", ");
    }

    let _complex = Complex {
        _name: String::from("Alℤiℤcℤe"),
        _ipaddr: _four,
        _hobbies: vec![String::from("baseball⚾"), "a static hobby".to_string(), String::from("⛽racing🏎️"), String::from("☃️🌨️skiing ⛷️"), "another static hobby".to_string(), String::from("hiking 🏔️")],
        _favorite_numbers: vec![40, 11, 45, 73],
        _address: "1234 home lane".to_string(),
        _static_state: "minnesota",
        _string_country: "USA".to_string(),
    };

    let _alice_unicode = String::from("Alℤiℤcℤe");
    let _string_messy = String::from("ℤ⛔mix⭐⚾⽸inℋ☕⛍⚻some⛲⤅⛄⋹Ω✨ascii✨ⱗ╁⏞⛽ⱴ");
    let _alice_ascii = String::from("Alice");
    let _vec_static_str = vec![pangram, pangram, pangram];
    let _vec_string = vec![_alice_unicode.clone(), _string_messy.clone(), _alice_ascii.clone()];
    let _arr_string = [_alice_unicode, _string_messy, _alice_ascii];
    let _inf = 100.0/0.0;
    let _inf_neg = -100.0/0.0;
    let _nan = (-1.0_f32).sqrt();
}

