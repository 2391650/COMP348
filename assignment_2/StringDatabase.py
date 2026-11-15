import random

class StringDatabase:
    def __init__(self, filename="four_letters.txt"):
        with open(filename, "r", encoding="utf-8") as word_file:
            contents = word_file.read()

        self.words = contents.lower().split()

        if len(self.words) == 0:
            raise ValueError("The word file is empty.")

        for word in self.words:
            if len(word) != 4:
                raise ValueError("Each word must have four letters.")

            for letter in word:
                if letter not in "abcdefghijklmnopqrstuvwxyz":
                    raise ValueError("Words must contain only letters from a to z.")

    def get_word(self):
        return random.choice(self.words)