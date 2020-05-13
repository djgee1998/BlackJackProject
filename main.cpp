#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iterator> 
#include <map>
#include <utility>
using namespace std;


struct Card {
	string value;
	string suit;
};

string printCard(Card c) {
	return (c.value + c.suit);
}


struct Deck {
	int currentCard = 0;
	Card card[52];
};

Deck create_deck() {
	const int NUMCARDS = 52;
	const int CARDSIZE = 2;
	Deck deck;
	char suits[] = { 'C','D','H','S' };
	char cardValues[] = { 'A','K','Q','J','T','9','8','7','6','5','4','3','2' };
	int count = 0;
	Card current;
	// for each suit
	for (int n = 0; n < 4; n++) {
		// for each of the unique card values
		for (int m = 0; m < 13; m++) {
			current.suit = suits[n];
			current.value = cardValues[m];
			deck.card[count] = current;
			count++;
		}
	}
	if (count == 52) {
		cout << "deck size OK..." << endl;
	}
	else {
		cout << "deck size: " << count << ", ERROR...exiting." << endl;
		exit(-1);
	}
	return deck;
}


struct Hand {
	string owner;
	int numCards = 0;
	Card card[12];
};


Hand createHand(string owner) {
	Hand h;
	h.owner = owner;
	return h;
}


void printHand(Hand h) {
	string s;
	cout << "\n" << h.owner << "'s hand:";
	for (int i = 0; i < h.numCards; i++) {
		s += (" " + printCard(h.card[i]));
	}
	cout << s << endl;
}

Hand addCard(Hand h, Card c) {

	if (h.numCards < 11) {
		h.card[h.numCards] = c;
		h.numCards++;

	}
	return h;
}


Hand dealCard(Hand h, Deck d) {
	int i = d.currentCard;
	h = addCard(h, d.card[d.currentCard]);
	return h;
}


// TODO: replace the cards suits in the deck with the symbols
Card suitReplace(Card c) {
	if (c.suit == "S") {
		c.suit = "♠";
	}
	else if (c.suit == "H") {
		c.suit = "♥";
	}
	else if (c.suit == "C") {
		c.suit = "♣";
	}
	else if (c.suit == "D") {
		c.suit = "♦";
	}
	else {
		cout << "ERROR: invalid suit value in card: " << c.value << c.suit << endl;
	}
	return c;
}


// TODO: Evaluate the hand, including Aces being worth 11 or 1 (if necessary)
int eval(Hand h) {
	int score = 0;

	map <string, int> cardMap = {
		{"A",11},
		{"K",10},
		{"Q",10},
		{"J",10},
		{"T",10},
		{"9",9},
		{"8",8},
		{"7",7},
		{"6",6},
		{"5",5},
		{"4",4},
		{"3",3},
		{"2",2}
	};

	// Print the cardVal map -- comment this out in your program 
	// cout << "\nThe map is:\n"; 
	// cout << "KEY\tVALUE\n";
	// for (const auto& [key, value] : cardMap){
	// 	cout << key << ":\t" << value << " points\n";
	// }

	// printHand(h);

	// for each card in the hand, add the score:
  // cout << "DEBUG: Looking for " << h.card[n].value << endl;
	for (int n = 0; n < h.numCards; n++) {
		auto search = cardMap.find(h.card[n].value);
		if (search != cardMap.end()) {
			score += static_cast<int>(search->second);
		}
		else {
			cout << "ERROR: key not found\n";
		}
	}

	return score;
}


void printDeck(Deck d) {
	for (int i = 0; i < 52; i++) {
		cout << d.card[i].value << d.card[i].suit;
		if (i != 51) { cout << ','; }
	}
	cout << endl;
	return;
}



// === MAIN === 
int main() {
	int againnumber;
	bool again;
	int player1Score = 0;
	int dealerScore = 0;
	int tieScore = 0;
	double totalGames;
	int playerPercent;
	int dealerPercent;
	int tiePercent;
	do {
		const int NUMCARDS = 52;
		const int CARDSIZE = 2;
		char hit;
		bool stand = false;
		Deck deck;
		Hand dealerHand = createHand("Dealer");
		Hand player1Hand = createHand("Player 1");

		deck = create_deck();
		// DEBUG: deck validation.
		// printDeck(deck); 

		// Replace the card letters with symbols
		for (int i = 0; i < 52; i++) {
			deck.card[i] = suitReplace(deck.card[i]);
		}
		
	  	// shuffle the deck
		string holder;
		string holder2;
		unsigned seed = time(0);
		srand(seed);
		int down = ((rand() % 25) + 26);
		for (int num = 0; num <= 25; num++) {
			holder = deck.card[num].value;
			holder2 = deck.card[num].suit;
			deck.card[num].value = deck.card[down].value;
			deck.card[num].suit = deck.card[down].suit;
			deck.card[down].value = holder;
			deck.card[down].suit = holder2;
			down++;
			if (down == 52) {
				down = 0;
			}
		}
		cout << "deck shuffled" << endl;
		// DEBUG: (showing the shuffled deck)
		// cout << "Shuffled deck: "  << endl; 
		// printDeck(deck);

		// Dealing cards out from the deck to make hands
		// The player gets dealt the first card.
		player1Hand = dealCard(player1Hand, deck);
		deck.currentCard++;
		dealerHand = dealCard(dealerHand, deck);
		deck.currentCard++;
		player1Hand = dealCard(player1Hand, deck);
		deck.currentCard++;
		dealerHand = dealCard(dealerHand, deck);
		deck.currentCard++;
		// cout << "DEBUG: Current number of cards dealt (deck list index): " << deck.currentCard << endl;
		int player1Handscore = eval(player1Hand);
		int dealerHandscore = eval(dealerHand);
		do {
			// Printing out the hands (hiding the hole card for the dealer)
			printHand(player1Hand);
			cout << player1Handscore;
			cout << "\nDealer cards: ?? ";
			for (int s = 1; s <= deck.currentCard; s++) {
				cout << dealerHand.card[s].value + dealerHand.card[s].suit << " ";
			}
			cout << endl;

			//debug to check the dealers hand
			// printHand(dealerHand); 

			hit = '1';
			if (eval(player1Hand) < 21) {
				player1Handscore = eval(player1Hand);
			}
			dealerHandscore = eval(dealerHand);
			if (player1Handscore == 21 && dealerHandscore != 21) {
				hit = '2';
			}
			else if (player1Handscore != 21 && dealerHandscore == 21) {
				printHand(player1Hand);
				printHand(dealerHand);
				cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
				cout << "Dealer Wins" << endl;
				dealerScore += 1;
				break;
			}
			else if ((player1Handscore == 21) && (dealerHandscore == 21)) {
				printHand(player1Hand);
				printHand(dealerHand);
				cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
				cout << "PUSH" << endl;
				tieScore += 1;
				break;
			}
			else if (player1Handscore > 21 && dealerHandscore < 21) {
				printHand(player1Hand);
				printHand(dealerHand);
				cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
				cout << "Dealer Wins" << endl;
				dealerScore += 1;
				break;
			}
			else if (dealerHandscore > 21 && player1Handscore < 21) {
				printHand(player1Hand);
				printHand(dealerHand);
				cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
				cout << "Player Wins" << endl;
				player1Score += 1;
				break;
			}
			if (!(hit == '2')) {
				cout << "\n1.Hit" << endl;
				cout << "2.Stand" << endl;
				cout << "[1/2]: ";
				cin >> hit;
			}
			if (hit == '2') {
				stand = true;
				if (eval(player1Hand) > eval(dealerHand)) {
					while (dealerHandscore <= 16) {
						dealerHand = dealCard(dealerHand, deck);
						deck.currentCard++;
						dealerHandscore = eval(dealerHand);
					}
				}
				printHand(player1Hand);
				printHand(dealerHand);
				if (player1Handscore == 21 && dealerHandscore != 21) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "You got Black Jack!" << endl;
					player1Score += 1;
					break;
				}
				else if (player1Handscore < 21 && dealerHandscore == 21) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "Dealer Wins" << endl;
					dealerScore += 1;
					break;
				}
				else if ((player1Handscore == 21) && (dealerHandscore == 21)) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "PUSH" << endl;
					tieScore += 1;
					break;
				}
				else if (player1Handscore > 21) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "Dealer Wins" << endl;
					dealerScore += 1;
					break;
				}
				else if (((player1Handscore > dealerHandscore) && player1Handscore < 21) || (dealerHandscore > 21)) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "Player Wins with the better hand!!" << endl;
					player1Score += 1;
					break;
				}
				else if (((dealerHandscore > player1Handscore) && dealerHandscore < 21) || (player1Handscore > 21)) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "Dealer Wins with the better hand!!" << endl;
					dealerScore += 1;
					break;
				}
				else if (player1Handscore == dealerHandscore) {
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "PUSH" << endl;
					tieScore += 1;
					break;
				}
				else {
					cout << "error\n";
				}
			}
			else if (hit == '1') {
				stand = true;
				player1Hand = dealCard(player1Hand, deck);
				deck.currentCard++;
				player1Handscore = eval(player1Hand);
				// Changes aces to 1 if player is over 21
				if (player1Handscore > 21) {
					for (int change = 0; change <= deck.currentCard; change++) {
						if (player1Hand.card[change].value == "A" && player1Handscore > 21) {
							player1Handscore -= 10;
							cout << "\nYour aces can now count as 1 (since you were over 21)\n";
						}
					}
				}
				if (player1Handscore > 21) {
					printHand(player1Hand);
					printHand(dealerHand);
					cout << "\nPlayer's Score: " << player1Handscore << "\nDealer's Score: " << dealerHandscore << endl;
					cout << "Dealer Wins" << endl;
					dealerScore += 1;
					break;
				}

			}
		} while (stand);
		totalGames = player1Score + dealerScore + tieScore;
		playerPercent = (player1Score / totalGames) * 100;
		dealerPercent = (dealerScore / totalGames) * 100;
		tiePercent = (tieScore / totalGames) * 100;
		cout << "************************************" << endl;
		cout << "Player 1 Games Won: " << player1Score << endl;
		cout << "Player 1 Winning Percentage: " << playerPercent << "%" << endl;
		cout << "Dealer's Games Won: " << dealerScore << endl;
		cout << "Dealer Winning Percentage: " << dealerPercent << "%" << endl;
		cout << "Ties: " << tieScore << endl;
		cout << "Percentage of Ties: " << tiePercent << "%" << endl;
		cout << "************************************" << endl;
		cout << "Would you like to play again?\n1.Yes\n2.No\n[1/2]: ";
		cin >> againnumber;
		if (againnumber == 1) {
			again = true;
		}
		else if (againnumber == 2) {
			again = false;
		}
	} while (again);
	totalGames = player1Score + dealerScore + tieScore;
	playerPercent = (player1Score / totalGames) * 100;
	dealerPercent = (dealerScore / totalGames) * 100;
	tiePercent = (tieScore / totalGames) * 100;
	cout << "\n************************************" << endl;
	cout << "************FINAL SCORE*************" << endl;
	cout << "Total Games: " << totalGames << endl;
	cout << "Player 1 Games Won: " << player1Score << endl;
	cout << "Player 1 Winning Percentage: " << playerPercent << "%" << endl;
	cout << "Dealer's Games Won: " << dealerScore << endl;
	cout << "Dealer Winning Percentage: " << dealerPercent << "%" << endl;
	cout << "Ties: " << tieScore << endl;
	cout << "Percentage of Ties: " << tiePercent << "%" << endl;
	cout << "************************************" << endl;
	cout << "************************************" << endl;


	return 0;
}
