use std::time::Duration;
use std::collections::HashMap;
// Demo/working program to find solutions to the HIQ peg puzzle, which
// starts with a cross shaped board filled with pegs except for one
// empty spot
// ..XXX..
// ..XXX..
// XXXXXXX
// XXX XXX
// ..XXX..
// ..XXX..
//
// with the usual goal of repeatedly jump one peg over an adjacent peg
// with the goal or reducing it to a single remaining peg.

#[derive(PartialEq, Clone, Copy)]
enum Peg{ EMPTY, PEG, OUT, }

#[derive(Debug)]
struct Move {
    i: usize,
    j: usize,
    dir: usize,
}


const NUM_PEGS: usize = 4*3 + 3*7 - 1;

const DELTA_I : [i32; 4] = [ 1, 0, -1, 0 ];
const DELTA_J : [i32; 4] = [ 0, 1, 0, -1 ];

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
    let mut board = init_board();
    let mut moves : Vec<Move> = Vec::new();

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

    let _alice = String::from("Alice");

    // just adding this so when running locally gdb4hpc is able to connect.
    std::thread::sleep(Duration::from_millis(5000));
    while next_move(&mut board, &mut moves) {
            //println!("{} moves", moves.len());
            //print_board(&board);
    }
}

fn init_board() -> [[Peg; 9]; 9] {
    let mut board : [[Peg; 9]; 9] = [[Peg::OUT; 9]; 9];
    let rngs : [[usize ; 2]; 9] = [ [10,10], [3,5], [3,5], [1,7], [1,7], [1,7], [3,5], [3,5], [10,10] ];

    for j in 0usize..9usize {
        let (min_c, max_c) = (rngs[j][0], rngs[j][1]);
        for i in 0usize..9usize {
            if i >= min_c && i <= max_c {
                board[j][i] = Peg::PEG;
            } 
        }
    }
    board[4][4] = Peg::EMPTY;
    return board
}

fn next_move(board : &mut[[Peg; 9]; 9], moves : &mut Vec<Move>) -> bool {

    use Peg::*;

    let mut i0 = 1;
    let mut j0 = 1;
    let mut dir0 = 0;
    loop {
        for j in j0..8 {
            for i in i0..8 {
                if board[j][i] != PEG { continue; }
                for dir in dir0..4 {
                    let (i1,j1) = offset(i,j,dir);
                    let (i2,j2) = offset(i1,j1,dir);
                    if board[j1][i1] == PEG &&
                       board[j2][i2] == EMPTY {
                        // got a move
                        board[j][i] = EMPTY;
                        board[j1][i1] = EMPTY;
                        board[j2][i2] = PEG;
                        let m = Move{ i: i, j: j, dir: dir};
                        moves.push(m);
                        return true;
                    }
                }
                dir0 = 0;
            }
        }

        if moves.len() == 0 {
            return false;
        }

        if moves.len() == NUM_PEGS-1 {
            println!("got a solution");
            return false;
        }

        let last = moves.pop().unwrap();
        let (i,j,dir) = (last.i, last.j, last.dir);
        let (i1,j1) = offset(i,j,dir);
        let (i2,j2) = offset(i1,j1,dir);

        board[j][i]   = PEG;
        board[j1][i1] = PEG;
        board[j2][i2] = EMPTY;

        i0 = i;
        j0 = j;
        dir0 = dir+1;
    } 
}

fn offset(i : usize, j : usize, dir : usize) -> ( usize, usize) {
    let di = DELTA_I[dir];
    let dj = DELTA_J[dir];

    let si = i as i32;
    let sj = j as i32;

    let oi = (si + di) as usize;
    let oj = (sj + dj) as usize;

    return (oi, oj)
}
    

fn _print_board(board : &[[Peg; 9]; 9]) {
    use Peg::*;

    for j in 0..9 {
        for i in 0..9 {
            let v = board[j][i];
            match v {
                 OUT => print!("."),
                 PEG => print!("X"),
                 EMPTY => print!(" "),
            }
        }
        println!("")
    }
}

