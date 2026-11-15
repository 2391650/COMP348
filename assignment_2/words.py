import sys
from Guess import Guess

def main():
    if len(sys.argv) != 2:
        print("Usage: python3 words.py play|test")
        return 1

    mode = sys.argv[1].lower()

    if mode != "play" and mode != "test":
        print("The mode must be play or test.")
        return 1

    try:
        guessing_game = Guess(mode)
    except (OSError, ValueError) as error:
        print("Could not load four_letters.txt:", error)
        return 1

    guessing_game.run()
    return 0

if __name__ == "__main__":
    sys.exit(main())