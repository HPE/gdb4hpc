import lib

def calc(lhs, rhs):
    answer = lhs + rhs
    return answer

def main():
    lhs = calc(1, 2)
    rhs = calc(3, lib.magic())
    answer = calc(lhs, rhs)
    print(answer)

if __name__ == "__main__":
    main()
