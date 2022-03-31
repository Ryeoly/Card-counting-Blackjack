#include <iostream>
#include <vector>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

struct Player {	// player1, 2�� ������ ��Ƶ� ����ü
	int win;
	int draw_lose;
	int money;
};

void Shuffle(vector<int> *card_deck) { // ī�带 �����ִ� ���

	card_deck->clear();

	for (int i = 0; i < 6; i++) {		// 6���� ���� ���
		for (int j = 0; j < 4; j++) {	// ī�� ������ 4���� ����
			card_deck->push_back(1);	// A
			card_deck->push_back(2);
			card_deck->push_back(3);
			card_deck->push_back(4);
			card_deck->push_back(5);
			card_deck->push_back(6);
			card_deck->push_back(7);
			card_deck->push_back(8);
			card_deck->push_back(9);
			card_deck->push_back(10);	// J
			card_deck->push_back(11);	// Q
			card_deck->push_back(12);	// K
			card_deck->push_back(13);
		}
	}

	std::random_device rd;
	std::mt19937 g(rd());

	std::shuffle(card_deck->begin(), card_deck->end(), g);	// ī�� ������ ����
}

double percent_cardcount(int card_count[14], int dealer_card) {
	int remain_number_of_card = 312 - card_count[13];		// ���� ī�� �� ����, �и� 
	int remain_card = 0;									// ����. ������ ��(��) ,ī���� ���� ����
	int use_card = 0;										// ī���� ���ڿ��� ����(��), ���ݱ��� ���� ī���� ���� 
	int tmp = 21 - (dealer_card + 10);						// ��� ��ġ���� card_counting�� ���� ����

	remain_card = (13 - tmp) * 6 * 4;						// 6�� ���� ����, 4�� ����� 4������.
	for (int i = tmp; i < 13; i++) {
		use_card += card_count[i];
	}

	double percent = ((double)remain_card - (double)use_card) / (double)remain_number_of_card;
	percent = percent * 100;								// burst percent

	return percent;
}

double percent_cardcount_for_player1(int card_count[14], int user_card) {
	int remain_number_of_card = 312 - card_count[13];		// ���� ī�� �� ����, �и� 
	int remain_card = 0;									// ����. ������ ��(��) ,ī���� ���� ����
	int use_card = 0;										// ī���� ���ڿ��� ����(��), ���ݱ��� ���� ī���� ���� 
	int tmp = 21 - user_card;								// ��� ��ġ���� card_counting�� ���� ����

	remain_card = (13 - tmp) * 6 * 4;						// 6�� ���� ����, 4�� ����� 4������.
	for (int i = tmp; i < 13; i++) {
		use_card += card_count[i];
	}

	double percent = ((double)remain_card - (double)use_card) / (double)remain_number_of_card;
	percent = percent * 100;								// burst percent

	return percent;
}

int SumofNum(vector<int>* field) { // ���� �÷��̾��� ī�� ���� ���� ���
	int sum_of_field = 0;
	int number_of_A = 0;

	for (int i = 0; i < field->size(); i++) {
		if (field->at(i) == 1) {		// A�� 1�� 11�� �� �� �ֱ⿡ A�� ī����
			number_of_A++;
		}
	}

	for (int i = 0; i < field->size(); i++) {
		if (field->at(i) >= 2 && field->at(i) <= 10) {	// 2~10 
			sum_of_field += field->at(i);
		}
		else if (field->at(i) > 10) {	// J, Q, K
			sum_of_field += 10;
		}
		else if (field->at(i) == 1) {	// A

		}
		else {
			cout << "������ �� ���� ī�尡 ����. error";
		}
	}

	int compare_one = sum_of_field;
	int compare_two = sum_of_field;
	int result_sum = sum_of_field;
	/* �ռ� A�� ������ �ʾҴ� ����. ���⼭ A�� ��� ��Ȳ�� ���� �� ��*/
	if (number_of_A > 0) {
		compare_one = compare_one + number_of_A * 1;				// all A is 1
		compare_two = compare_two + (number_of_A - 1) * 1 + 11;		// 11 �Ѱ�, ������ 1

		for (int i = 0; i < number_of_A; i++) {						// A�� ��� ����� ��
			/* A�� �̿� ��� �� ������ ��Ȳ�� ã�� �� */
			if (compare_one > 21 && compare_two > 21) {				
				result_sum = compare_one;
			}
			else if (compare_one > 21) {
				result_sum = compare_two;
			}
			else if (compare_two > 21) {
				result_sum = compare_one;
			}
			else if (compare_one < 22 && compare_two < 22) {		// �Ѵ� burst���� ���� �� 21�� ����� ���� ã��
				int tmp = 21 - compare_one;
				int tmp2 = 21 - compare_two;
				if (tmp < tmp2) {
					result_sum = compare_one;
				}
				else if (tmp > tmp2) {
					result_sum = compare_two;
				}
				else {
					result_sum = compare_one;
				}
			}

			compare_one = result_sum;								// ���� ��Ȳ�� ���� ������ ��
			compare_two = sum_of_field;								// ���� ��Ȳ�� ���� ��
			compare_two = compare_two + (number_of_A - (i + 2)) * 1 + 11 * (i + 2);	// ���� ���� ��Ȳ�� ã�� ��
		}
	}

	return result_sum;
}

					/*!!!!!!!!!!!!!!1 false = stand, true = hit !!!!!!!!!!!!!!!11!*/
bool HitStatus(int who, int dealer_card, vector<int> *field, int card_count[14]) { //ī�� Hit, Stand �����ִ� ���.
	int sum_card = 0;
	sum_card = SumofNum(field);
	
	// dealer�� player2�� who = 0
	if (who == 0) {
		if (sum_card >= 17) {
			return false;
		}
		else if (sum_card < 17) {
			return true;
		}
		else {
			cout << "dealer, player2 sum_card error";
			return true;
		}
	}

	// player1. �� Ȯ�� ����ϴ� �ִ� who = 1
	else if (who == 1) {
		double burst_percent = percent_cardcount(card_count, dealer_card);				// ������ ����Ʈ�� Ȯ�� 
		double user_bust_percent = percent_cardcount_for_player1(card_count, sum_card); // player1�� ����Ʈ�� Ȯ��

		/* �� ��Ȳ�� ���� return���� ������ ��� */
		if (dealer_card + 10 <= 16) {		// ������ ���� �а� 6������ ���
			if (burst_percent >= 70) {
				return false;
			}
			else {
				if (sum_card < dealer_card + 10) {
					if (user_bust_percent >= 70) {
						return false;
					}
					else {
						return true;
					}
				}
				else {
					return false;
				}
			}
		}
		else if (dealer_card + 10 >= 17) {		// ������ �����а� 7�̻��� ���
			if (sum_card < dealer_card + 10) {
				if (user_bust_percent >= 70) {
					return false;
				}
				else {
					return true;
				}
			}
			else {
				return false;
			}
		}
		else {									// ���� ó��
			if (user_bust_percent >= 70) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	else {	// ���� ó��
		cout << "HitStatus error. �߸��� game player ����";
		return true;
	}

	cout << "hitstatus error";
	return false;		//������� ���� 99.99% ����
}



void start_game(struct Player *player1, struct Player* player2, int number_of_game) {

	vector<int> card_deck;			// �÷����� ī�� ����
	int card_count[14] = { 0, };	// ī�带 ī������ �迭

	int deck_20percent = 6 * 52 * 0.2;	// ���ø��� ����. 6������ 20�ۼ�Ʈ�� ������ ���ý�Ų��.

	vector<int> player1_field;
	vector<int> player2_field;
	vector<int> dealer_field;

	int turn_of_card = 0;			// ī�带 ������ �� �� � ī������ ������ ����

	for (int i = 0; i < number_of_game; i++) {

		if (card_deck.size() <= deck_20percent) {
			Shuffle(&card_deck);
			for (int j = 0; j < 14; j++) {
				card_count[j] = 0;
			}
		}

		// field clear. ���ο� ������ ������ �غ�
		dealer_field.clear();		
		player1_field.clear();
		player2_field.clear();

					/* game start */
		// ������ �׻� 100������ ���� 
		player1->money = player1->money - 100;
		player2->money = player2->money - 100;

		// 2���� ī�带 ������� �������ش�. 
		for (int j = 0; j < 2; j++) {	
			turn_of_card = card_deck.back();
			card_deck.pop_back();
			dealer_field.push_back(turn_of_card);
			if (j == 1) {							// ������ �ι�° ���� ī�常 ����
				card_count[turn_of_card - 1]++;		// card count
				card_count[13]++;					// total count			
			}

			turn_of_card = card_deck.back();
			card_deck.pop_back();
			player1_field.push_back(turn_of_card);
			card_count[turn_of_card - 1]++;		// card count
			card_count[13]++;					// total count

			turn_of_card = card_deck.back();
			card_deck.pop_back();
			player2_field.push_back(turn_of_card);
			card_count[turn_of_card - 1]++;		// card count
			card_count[13]++;					// total count
		}
		


			/* player1, player2, dealer ������� ī�带 ������ ���� �����ϸ� ���� */
		bool hitstatus = HitStatus(1, dealer_field.at(1), &player1_field, card_count);
		while (hitstatus) {
			turn_of_card = card_deck.back();
			card_deck.pop_back();
			player1_field.push_back(turn_of_card);
			card_count[turn_of_card - 1]++;		// card count
			card_count[13]++;					// total count
			hitstatus = HitStatus(1, dealer_field.at(1), &player1_field, card_count);
		}

		hitstatus = HitStatus(0, dealer_field.at(1), &player2_field, card_count);
		while (hitstatus) {
			turn_of_card = card_deck.back();
			card_deck.pop_back();
			player2_field.push_back(turn_of_card);
			card_count[turn_of_card - 1]++;		// card count
			card_count[13]++;					// total count
			hitstatus = HitStatus(0, dealer_field.at(1), &player2_field, card_count);
		}

		hitstatus = HitStatus(0, dealer_field.at(1), &dealer_field, card_count);
		while (hitstatus) {
			turn_of_card = card_deck.back();
			card_deck.pop_back();
			dealer_field.push_back(turn_of_card);
			card_count[turn_of_card - 1]++;		// card count
			card_count[13]++;					// total count
			hitstatus = HitStatus(0, dealer_field.at(1), &dealer_field, card_count);
		}


						/* ���� ����, �� �й� */
		int player1_end = SumofNum(&player1_field);
		int player2_end = SumofNum(&player2_field);
		int dealer_end = SumofNum(&dealer_field);

		if (player1_end > 21) {
			// player1 �й�
			player1->draw_lose++;
		}
		else if (player1_end <= 21) {
			if (dealer_end > 21) {
				if (player1_field.size() == 2 && player1_end == 21) {	// �������� �̱�� ���
					player1->money += 250;
					player1->win++;
				}
				else {		// �׳� �̱�� ���
					player1->money += 200;
					player1->win++;
				}
			}
			else if (dealer_end <= 21) {
				if (player1_end > dealer_end) {		
					if (player1_field.size() == 2 && player1_end == 21) { // �������� �̱�� ���
						player1->money += 250;
						player1->win++;
					}
					else {	// �׳� �̱�� ���
						player1->win++;
						player1->money += 200;
					}
				}
				else {
					// player1 �й�
					player1->draw_lose++;
				}
			}
		}


		if (player2_end > 21) {
			// player2 �й�
			player2->draw_lose++;
		}
		else if (player2_end <= 21) {
			if (dealer_end > 21) {
				if (player2_field.size() == 2 && player2_end == 21) {	// �������� �¸�
					player2->money += 250;
					player2->win++;
				}
				else {	// �׳� �¸�
					player2->money += 200;
					player2->win++;
				}
			}
			else if (dealer_end <= 21) {
				if (player2_end > dealer_end) {
					if (player2_field.size() == 2 && player2_end == 21) {	//���� �¸�
						player2->money += 250;
						player2->win++;
					}
					else {	// �׳� �¸�
						player2->win++;
						player2->money += 200;
					}
				}
				else {
					// player2 �й�
					player2->draw_lose++;
				}
			}
		}
	}

}

int main()
{
	int number_of_game = 0;	
	cout << "Play Game : ";
	cin >> number_of_game;		// ���� Ƚ�� �Է�

	/* player1, player2 ���� ���� */
	struct Player player1;
	player1.win = 0;
	player1.draw_lose = 0;
	player1.money = 100000;

	struct Player player2;
	player2.win = 0;
	player2.draw_lose = 0;
	player2.money = 100000;

	/* ���� ���� */
	start_game(&player1, &player2, number_of_game);

	/* ���� ��� ��� */
	cout << "<< Player1 >>" << endl;
	cout << "�¸� : " << player1.win << " ���º� �� �й� : " << player1.draw_lose << endl;
	cout << "money : " << player1.money << endl << endl;

	cout << "<< Player2 >>" << endl;
	cout << "�¸� : " << player2.win << " ���º� �� �й� : " << player2.draw_lose << endl;
	cout << "money : " << player2.money << endl << endl;

	return 0;
}