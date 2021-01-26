
// RNPC ERROR CODES

#define NO_ERROR 0
#define INVALID_ARGS -1
#define STACK_TO_LARGE -2
#define STACK_TO_SMALL -3
#define INVALID_OPERAND -4
#define DIVIDED_BY_ZERO -5
#define STACK_UNDERFLOW -6
#define STACK_OVERFLOW -7
#define NO_FILE -8

// comment out debug to turn DEBUG_PRINT off
//#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(...) do{ printf(__VA_ARGS__ ); } while( false )
#else
#define DEBUG_PRINT(...) do{ } while ( false )
#endif

class Stack{

	private:
		// stack variables
		int s;
		int top;
		double *stk;
	public:
		// stack functions
		Stack();
		Stack(int size);	
		bool push(double value);
		double pop(void);	
		bool empty(void);
		bool full(void);
		void clear();
		void dumpStack(int size);
};
