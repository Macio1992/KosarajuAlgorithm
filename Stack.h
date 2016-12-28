#ifndef STACK_H
#define STACK_H

class Stack {
	
	private:
		int *stack;
		int size;
		int top;
		
	public:
		Stack(int);
		~Stack();
		void push(int);
		void pop();
		void display();
		int getStackTop();
		bool checkIfStackEmpty();
};

#endif
