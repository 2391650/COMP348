import os
import sys
import subprocess
from Game import Game
from StringDatabase import StringDatabase

class Guess:
    FREQUENCIES = {
        "a": 8.17, "b": 1.49, "c": 2.78, "d": 4.25, "e": 12.70,
        "f": 2.23, "g": 2.02, "h": 6.09, "i": 6.97, "j": 0.15,
        "k": 0.77, "l": 4.03, "m": 2.41, "n": 6.75, "o": 7.51,
        "p": 1.93, "q": 0.10, "r": 5.99, "s": 6.33, "t": 9.06,
        "u": 2.76, "v": 0.98, "w": 2.36, "x": 0.15, "y": 1.97,
        "z": 0.07
    }

    def __init__(self, mode):
        self.mode = mode
        self.database = StringDatabase()
        self.completed_games = []

        word = self.database.get_word()
        self.current_game = Game(word)

    def run(self):
        try:
            while True:
                self.display_menu()
                option = input("Enter Option: ").strip().lower()

                while option not in ("g", "t", "l", "q"):
                    option = input(
                        "Invalid option. Please re-enter: "
                    ).strip().lower()

                if option == "q":
                    break
                elif option == "g":
                    self.guess_word()
                elif option == "t":
                    self.tell_me()
                elif option == "l":
                    self.guess_letter()

                input("Press any key to continue... ")

        except (EOFError, KeyboardInterrupt):
            print()

        self.display_report()

    def clear_screen(self):
        if sys.stdout.isatty():
            if os.name == "nt":
                subprocess.run("cls", shell=True)
            else:
                # In case of Linux
                subprocess.run(["clear"])

    def display_menu(self):
        self.clear_screen()
        print("++")
        print("++ The great guessing game")
        print("++")
        print()

        if self.mode == "test":
            print("Current Word:", self.current_game.word)

        print("Current Guess:", self.current_guess())
        print("Letters guessed:", " ".join(self.current_game.letters_guessed))
        print()
        print("g = guess, t = tell me, l for a letter, and q to quit")
        print()

    def display_feedback(self, message):
        print()
        print("@@")
        print("@@ FEEDBACK:", message)
        print("@@")
        print()

    def current_guess(self):
        hidden_word = ""

        for letter in self.current_game.word:
            if letter in self.current_game.letters_guessed:
                hidden_word += letter
            else:
                hidden_word += "-"

        return hidden_word

    def read_letters(self, prompt, length):
        while True:
            answer = input(prompt).strip().lower()
            valid = True

            if len(answer) != length:
                valid = False

            for letter in answer:
                if letter not in "abcdefghijklmnopqrstuvwxyz":
                    valid = False
                    break

            if valid:
                return answer

            print("Please enter exactly", length, "letter(s) from a to z.")

    def guess_letter(self):
        letter = self.read_letters("Enter a letter: ", 1)
        game = self.current_game

        game.letter_requests += 1

        if letter not in game.letters_guessed:
            game.letters_guessed.append(letter)

        matches = game.word.count(letter)

        if matches == 0:
            game.missed_letters += 1
            self.display_feedback("Not a single match, genius")
        else:
            self.display_feedback(
                "Woo hoo, you found " + str(matches) + " letters"
            )

    def guess_word(self):
        answer = self.read_letters("Make your guess: ", 4)

        if answer == self.current_game.word:
            self.display_feedback("You're right, Einstein!")
            self.finish_game(True)
        else:
            self.current_game.bad_guesses += 1
            self.display_feedback("Try again, Loser!")

    def tell_me(self):
        self.display_feedback(
            "You really should have guessed this...'"
            + self.current_game.word + "'"
        )
        self.finish_game(False)

    def calculate_score(self, success):
        game = self.current_game
        remaining_points = 0.0

        for letter in game.word:
            if letter not in game.letters_guessed:
                remaining_points += self.FREQUENCIES[letter]

        if remaining_points == 0:
            return 0.0

        if not success:
            return -remaining_points

        score = remaining_points

        if game.letter_requests > 0:
            score = score / game.letter_requests

        penalty = game.bad_guesses * 0.10

        if penalty > 1:
            penalty = 1

        score = score * (1 - penalty)
        return score

    def finish_game(self, success):
        game = self.current_game

        if success:
            game.status = "Success"
        else:
            game.status = "Gave up"

        game.score = self.calculate_score(success)
        self.completed_games.append(game)

        word = self.database.get_word()
        self.current_game = Game(word)

    def display_report(self):
        self.clear_screen()
        print("++")
        print("++ Game Report")
        print("++")
        print()
        print(f"{'Game':<6}{'Word':<8}{'Status':<12}"
              f"{'Bad Guesses':>13}{'Missed Letters':>16}{'Score':>10}")
        print("-" * 65)

        total_score = 0.0
        game_number = 1

        for game in self.completed_games:
            print(f"{game_number:<6}{game.word:<8}{game.status:<12}"
                  f"{game.bad_guesses:>13}{game.missed_letters:>16}"
                  f"{game.score:>10.2f}")

            total_score += game.score
            game_number += 1

        print(f"\nFinal Score: {total_score:.2f}")