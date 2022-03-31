#include <iostream>
#include <vector>
#include <stdlib.h>
#include <random>
#include <time.h>
#include <algorithm>

using namespace std;

struct Player {	// player1, 2의 정보를 담아둘 구조체
	int win;
	int draw_lose;
	int money;
};

void Shuffle(vector<int> *card_deck) { // 카드를 섞어주는 기능

	card_deck->clear();

	for (int i = 0; i < 6; i++) {		// 6개의 덱을 사용
		for (int j = 0; j < 4; j++) {	// 카드 문양이 4가지 종류
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

	std::shuffle(card_deck->begin(), card_deck->end(), g);	// 카드 무작위 셔플
}

double percent_cardcount(int card_count[14], int dealer_card) {
	int remain_number_of_card = 312 - card_count[13];		// 남은 카드 총 개수, 분모 
	int remain_card = 0;									// 분자. 빼지는 것(앞) ,카운팅 남은 개수
	int use_card = 0;										// 카운팅 분자에서 빼줄(뒤), 지금까지 나온 카드의 개수 
	int tmp = 21 - (dealer_card + 10);						// 어느 위치부터 card_counting을 할지 정함

	remain_card = (13 - tmp) * 6 * 4;						// 6은 덱의 개수, 4는 모양이 4개여서.
	for (int i = tmp; i < 13; i++) {
		use_card += card_count[i];
	}

	double percent = ((double)remain_card - (double)use_card) / (double)remain_number_of_card;
	percent = percent * 100;								// burst percent

	return percent;
}

double percent_cardcount_for_player1(int card_count[14], int user_card) {
	int remain_number_of_card = 312 - card_count[13];		// 남은 카드 총 개수, 분모 
	int remain_card = 0;									// 분자. 빼지는 것(앞) ,카운팅 남은 개수
	int use_card = 0;										// 카운팅 분자에서 빼줄(뒤), 지금까지 나온 카드의 개수 
	int tmp = 21 - user_card;								// 어느 위치부터 card_counting을 할지 정함

	remain_card = (13 - tmp) * 6 * 4;						// 6은 덱의 개수, 4는 모양이 4개여서.
	for (int i = tmp; i < 13; i++) {
		use_card += card_count[i];
	}

	double percent = ((double)remain_card - (double)use_card) / (double)remain_number_of_card;
	percent = percent * 100;								// burst percent

	return percent;
}

int SumofNum(vector<int>* field) { // 현재 플래이어의 카드 숫자 합을 계산
	int sum_of_field = 0;
	int number_of_A = 0;

	for (int i = 0; i < field->size(); i++) {
		if (field->at(i) == 1) {		// A는 1과 11이 될 수 있기에 A를 카운팅
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
			cout << "존재할 수 없는 카드가 존재. error";
		}
	}

	int compare_one = sum_of_field;
	int compare_two = sum_of_field;
	int result_sum = sum_of_field;
	/* 앞서 A를 더하지 않았던 이유. 여기서 A의 모든 상황을 비교해 줄 것*/
	if (number_of_A > 0) {
		compare_one = compare_one + number_of_A * 1;				// all A is 1
		compare_two = compare_two + (number_of_A - 1) * 1 + 11;		// 11 한개, 나머지 1

		for (int i = 0; i < number_of_A; i++) {						// A의 모든 경우의 수
			/* A를 이용 방법 중 최적의 상황을 찾는 중 */
			if (compare_one > 21 && compare_two > 21) {				
				result_sum = compare_one;
			}
			else if (compare_one > 21) {
				result_sum = compare_two;
			}
			else if (compare_two > 21) {
				result_sum = compare_one;
			}
			else if (compare_one < 22 && compare_two < 22) {		// 둘다 burst되지 않을 때 21에 가까운 수를 찾음
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

			compare_one = result_sum;								// 이전 상황중 가장 최적의 패
			compare_two = sum_of_field;								// 다음 상황을 비교할 패
			compare_two = compare_two + (number_of_A - (i + 2)) * 1 + 11 * (i + 2);	// 이후 비교할 상황을 찾는 것
		}
	}

	return result_sum;
}

					/*!!!!!!!!!!!!!!1 false = stand, true = hit !!!!!!!!!!!!!!!11!*/
bool HitStatus(int who, int dealer_card, vector<int> *field, int card_count[14]) { //카드 Hit, Stand 정해주는 기능.
	int sum_card = 0;
	sum_card = SumofNum(field);
	
	// dealer와 player2는 who = 0
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

	// player1. 즉 확률 기반하는 애는 who = 1
	else if (who == 1) {
		double burst_percent = percent_cardcount(card_count, dealer_card);				// 딜러가 버스트할 확률 
		double user_bust_percent = percent_cardcount_for_player1(card_count, sum_card); // player1이 버스트할 확률

		/* 각 상황에 따른 return값은 보고서에 기술 */
		if (dealer_card + 10 <= 16) {		// 딜러의 오픈 패가 6이하인 경우
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
		else if (dealer_card + 10 >= 17) {		// 딜러의 오픈패가 7이상인 경우
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
		else {									// 예외 처리
			if (user_bust_percent >= 70) {
				return false;
			}
			else {
				return true;
			}
		}
	}
	else {	// 예외 처리
		cout << "HitStatus error. 잘못된 game player 들어옴";
		return true;
	}

	cout << "hitstatus error";
	return false;		//여기까지 오면 99.99% 에러
}



void start_game(struct Player *player1, struct Player* player2, int number_of_game) {

	vector<int> card_deck;			// 플레이할 카드 더미
	int card_count[14] = { 0, };	// 카드를 카운팅할 배열

	int deck_20percent = 6 * 52 * 0.2;	// 셔플링할 기준. 6개덱의 20퍼센트가 남으면 셔플시킨다.

	vector<int> player1_field;
	vector<int> player2_field;
	vector<int> dealer_field;

	int turn_of_card = 0;			// 카드를 나누어 줄 때 어떤 카드인지 저장할 변수

	for (int i = 0; i < number_of_game; i++) {

		if (card_deck.size() <= deck_20percent) {
			Shuffle(&card_deck);
			for (int j = 0; j < 14; j++) {
				card_count[j] = 0;
			}
		}

		// field clear. 새로운 게임을 진행할 준비
		dealer_field.clear();		
		player1_field.clear();
		player2_field.clear();

					/* game start */
		// 배팅은 항상 100원으로 고정 
		player1->money = player1->money - 100;
		player2->money = player2->money - 100;

		// 2장의 카드를 순서대로 나누어준다. 
		for (int j = 0; j < 2; j++) {	
			turn_of_card = card_deck.back();
			card_deck.pop_back();
			dealer_field.push_back(turn_of_card);
			if (j == 1) {							// 딜러는 두번째 받은 카드만 노출
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
		


			/* player1, player2, dealer 순서대로 카드를 받을지 말지 결정하며 진행 */
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


						/* 승패 결정, 돈 분배 */
		int player1_end = SumofNum(&player1_field);
		int player2_end = SumofNum(&player2_field);
		int dealer_end = SumofNum(&dealer_field);

		if (player1_end > 21) {
			// player1 패배
			player1->draw_lose++;
		}
		else if (player1_end <= 21) {
			if (dealer_end > 21) {
				if (player1_field.size() == 2 && player1_end == 21) {	// 블랙잭으로 이기는 경우
					player1->money += 250;
					player1->win++;
				}
				else {		// 그냥 이기는 경우
					player1->money += 200;
					player1->win++;
				}
			}
			else if (dealer_end <= 21) {
				if (player1_end > dealer_end) {		
					if (player1_field.size() == 2 && player1_end == 21) { // 블랙잭으로 이기는 경우
						player1->money += 250;
						player1->win++;
					}
					else {	// 그냥 이기는 경우
						player1->win++;
						player1->money += 200;
					}
				}
				else {
					// player1 패배
					player1->draw_lose++;
				}
			}
		}


		if (player2_end > 21) {
			// player2 패배
			player2->draw_lose++;
		}
		else if (player2_end <= 21) {
			if (dealer_end > 21) {
				if (player2_field.size() == 2 && player2_end == 21) {	// 블랙잭으로 승리
					player2->money += 250;
					player2->win++;
				}
				else {	// 그냥 승리
					player2->money += 200;
					player2->win++;
				}
			}
			else if (dealer_end <= 21) {
				if (player2_end > dealer_end) {
					if (player2_field.size() == 2 && player2_end == 21) {	//블랙잭 승리
						player2->money += 250;
						player2->win++;
					}
					else {	// 그냥 승리
						player2->win++;
						player2->money += 200;
					}
				}
				else {
					// player2 패배
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
	cin >> number_of_game;		// 게임 횟수 입력

	/* player1, player2 정보 세팅 */
	struct Player player1;
	player1.win = 0;
	player1.draw_lose = 0;
	player1.money = 100000;

	struct Player player2;
	player2.win = 0;
	player2.draw_lose = 0;
	player2.money = 100000;

	/* 게임 시작 */
	start_game(&player1, &player2, number_of_game);

	/* 게임 결과 출력 */
	cout << "<< Player1 >>" << endl;
	cout << "승리 : " << player1.win << " 무승부 및 패배 : " << player1.draw_lose << endl;
	cout << "money : " << player1.money << endl << endl;

	cout << "<< Player2 >>" << endl;
	cout << "승리 : " << player2.win << " 무승부 및 패배 : " << player2.draw_lose << endl;
	cout << "money : " << player2.money << endl << endl;

	return 0;
}