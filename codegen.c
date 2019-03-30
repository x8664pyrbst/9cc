#include "9cc.h"

void gen_lval(Node *node){
	if(node->ty != ND_IDENT){
		fprintf(stderr,"代入の左辺値が変数ではありません");
		exit(1);
	}
	if(!map_get(val_map,node->name)){
		fprintf(stderr,"未定義の変数です\n");
		exit(1);
	}
	int offset = (int)map_get(val_map,node->name); 
	printf("	mov rax, rbp\n");
	printf("	sub rax, %d\n",offset);
	printf("	push rax\n");
}

void gen(Node *node){
	if(node->ty == ND_NUM){
		printf("	push %d\n",node->val);
		return;
	}

	if(node->ty == ND_IDENT){
		gen_lval(node);
		printf("	pop rax\n");
		printf("	mov rax, [rax]\n");
		printf("	push rax\n");
		return;
	}
	if(node->ty == ND_FUNC){
		printf("	call %s\n",node->lhs->name);
		return;
	}

	if(node->ty == '='){
		gen_lval(node->lhs);
		gen(node->rhs);

		printf("	pop rdi\n");
		printf("	pop rax\n");
		printf("	mov [rax], rdi\n");
		printf("	push rdi\n");
		return;
	}
	if(node->ty != ND_FUNC){
		gen(node->lhs);
		gen(node->rhs);
	}

	printf("	pop rdi\n");
	printf("	pop rax\n");

	if(node->ty == ND_EQ){
		printf("	cmp rdi, rax\n");
		printf("	sete al\n");
		printf("	movzb rax, al\n");
	}
	else if(node->ty == ND_NEQ){
		printf("	cmp rdi, rax\n");
		printf("	setne al\n");
		printf("	movzb rax, al\n");//５６ビットをクリア
	}

	switch(node->ty){
		case '+':
			printf("	add rax, rdi\n");
			break;
		case '-':
			printf("	sub rax, rdi\n");
			break;
		case '*':
			printf("	mul rdi\n");
			break;
		case '/':
			printf("	mov rdx, 0\n");
			printf("	div rdi\n");
	}
	printf("	push rax\n");
}
