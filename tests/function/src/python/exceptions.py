def oofdah():
    empty_dict = {}
    print(empty_dict["oofdah"])

def main():
    safe_oofdah()
    oofdah()

def safe_oofdah():
    empty_dict = {}
    try:
        print(empty_dict["safe_oofdah"])
    except:
        pass

if __name__ == "__main__":
    main()
