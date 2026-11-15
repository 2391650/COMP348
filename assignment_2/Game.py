class Game:
    def __init__(self, word):
        self.word = word
        self.letters_guessed = []
        self.letter_requests = 0
        self.bad_guesses = 0
        self.missed_letters = 0
        self.status = "In progress"
        self.score = 0.0