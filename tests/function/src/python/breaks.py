import lib

def calc(lhs, rhs):
    answer = lhs + rhs
    return answer

def breaks_main():
    lhs = calc(1, 2)
    rhs = calc(3, lib.magic())
    answer = calc(lhs, rhs)
    print(answer)

if __name__ == "__main__":
    breaks_main()
