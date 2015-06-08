#include "interactive_horse_race.h"

#define MATH_QUESTIONS_LEN 256
#define CONVERSION_QUESTIONS_LEN 64
#define CANNED_QUESTIONS_LEN 9

luv_question_t math_questions[MATH_QUESTIONS_LEN];
luv_question_t conversion_questions[CONVERSION_QUESTIONS_LEN];

const luv_question_t canned_questions[CANNED_QUESTIONS_LEN] = {
  { "You wake up in a forrest and are surrounded by vines. A gate is to the north." , "N" }     ,
  { "What is the library that powers Node.js"   , "libuv" },
  { "What is the language libuv is implemented in", "C" },
  { "What is the language v8 is implemented in", "C++" },
  { "At what conference was learnuv first used as a workshopper", "campjs" },
  { "What is the first name of the campjs curator", "Tim" },
  { "What is the first name of the Node.js creator", "Ryan" },
  { "C function used to allocate memory", "malloc" },
  { "C function used to release memory", "free" },
};

static luv_question_t get_canned_question() {
  return canned_questions[rand() % CANNED_QUESTIONS_LEN];
}

static luv_question_t get_conversion_question() {
  return conversion_questions[rand() % CONVERSION_QUESTIONS_LEN];
}

static luv_question_t get_math_question() {
  return math_questions[rand() % MATH_QUESTIONS_LEN];
}

static void init_math_questions() {
  static const char *ops = "+-*";
  int i, p1, p2, result, max;
  char op;
  luv_question_t* qs;

  for (i = 0; i < MATH_QUESTIONS_LEN; i++) {
    op = ops[rand() % 3];
    max = op == '*' ? 10 : 100;
    p1 = rand() % max;
    p2 = rand() % max;

    switch(op) {
      case '+' : {
        result = p1 + p2;
        break;
      }
      case '-' : {
        result = p1 - p2;
        break;
      }
      default  : result = p1 * p2;
    }

    qs = &math_questions[i];
    snprintf(qs->question, QUESTION_LEN, "%d %c %d =", p1, op, p2);
    snprintf(qs->answer, QUESTION_LEN, "%d", result);
  }
}

static void init_conversion_questions() {
  int i;
  luv_question_t* qs;

  for (i = 0; i < CONVERSION_QUESTIONS_LEN; i += 2) {
    qs = &conversion_questions[i];

    snprintf(qs->question, QUESTION_LEN, "%d converted to HEXADECIMAL", i);
    snprintf(qs->answer, QUESTION_LEN, "%x", i);

    qs = &conversion_questions[i + 1];
    snprintf(qs->question, QUESTION_LEN, "0x%x converted to DECIMAL", i);
    snprintf(qs->answer, QUESTION_LEN, "%d", i);
  }
}

void luv_questions_init() {
  init_math_questions();
  init_conversion_questions();
}

luv_question_t luv_questions_get() {
  int r = rand() % 5;
  switch (r) {
    case 0  : return get_canned_question();

    /* the conversion questions are kinda hard, so we favor math questions ;) */
    case 1  :
    case 2  :
    case 3  : return get_math_question();

    default : return get_conversion_question();
  }
}
