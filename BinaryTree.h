#pragma once

#include<assert.h>
#include<stdio.h>
#include<stdlib.h>
typedef int TDataType;

//利用前序序列创建二叉树
//1.单纯用前序序列无法还原二叉树
//2.利用带空结点的前序

//二叉树的结构体   自身结点的数据 和左右结点(同一个类型)
typedef struct BNode{
	TDataType data;
	struct BNode *left;
	struct BNode *right;
}BNode;


//根 用过的结点
typedef struct
{
	BNode *root;
	int used;
}RESULT;




//创建结点 (开房子):
//1.开辟空间 2.放数据 3.指针初始化
static BNode *CreatNode(TDataType data)
{
	BNode *node = (BNode*)malloc(sizeof(BNode));
	node->data = data;
	node->left = node->right = NULL;
}





//利用含空结点的前序创建二叉树
//返回结果是二叉树的根节点
//preorder: 带空结点的前序()
//size: preorder的长度
//难点:1.递归
//     2.创建树的过程中会返回使用过的结点个数
RESULT  CreateTree(TDataType preorder[], int size)
{
	//次数用完 结束
	if (size == 0)
	{
		RESULT result = { NULL, 0 };
		return result;
	}

	//顺序:   根   ,   左子树    ,右子树
	//数组用来保存值
	//判断值是否-1
	TDataType rootValue = preorder[0];
	if(rootValue == -1)
	{
		//如果是-1 返回空指针 和 使用结点个数1
		RESULT result = { NULL, 1 };
		return result;
	}

	//值->创建结点
	BNode *root = CreatNode(rootValue);

	//左用 lr将1值传入,size-1
	RESULT lr = CreateTree(&preorder[1], size - 1);
	root->left = lr.root;
	//右用
	RESULT rr = CreateTree(&preorder[1 + lr.used], size - 1 - lr.used);
	root->right = rr.root;

	
	RESULT result = { root, 1 + lr.used + rr.used };
	return result;
}




//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//                                 二叉树问题思考方式 :                 1.递推公式:       2.终止条件:
//求结点个数
//终止条件: 
//1)空树 return 0
//2)叶子结点 return 1
//3)一个孩子为空
int GetSize(BNode *root)
{
	//终止条件
	if (root == NULL)
	{
		//空树
		return 0;
	}
	//递推
	return GetSize(root->left) +
		GetSize(root->right) + 1;
}

//求叶子结点个数
//空树 return 0
//只有一个结点的树 return 1
int GetLeafSize(BNode *root)
{
	if (root == NULL)
	{
		return 0;
	}
	else if (root->left == NULL && root->right == NULL)
	{
		return 1;
	}
	else
	{
		return GetLeafSize(root->left) +
			GetLeafSize(root->right) ;
	}
}

//求第k层结点个数
//1.递推
//    左+右
//    子问题在递推的过程需要
//    k-1
//2.终止条件
//    1)空树 return 0
//    2)k==1 return 1
int GetKLevelSize(BNode*root, int k)
{
	assert(k > 0);

	//位置不能交换 自己想为什么
	if (root == NULL)
	{
		return 0;
	}
	if (k == 1)
	{
		return 1;
	}

	return GetKLevelSize(root->left, k - 1) + 
		GetKLevelSize(root->right, k - 1);
}

//求二叉树的高度
//递推公式:
//         MAX(左,右)+1
//终止条件:
//         空树 return 0
#define MAX(a,b) ((a)>(b)?(a):(b))

int GetHeight(BNode *root)
{
	if (root == NULL)
	{
		return 0;
	}

	int leftHeight = GetHeight(root->left);
	int rightHeight = GetHeight(root->right);

	//注意下,不要多次调用
	return MAX(leftHeight, rightHeight) + 1;
}

//查找结点:
//      如果找到,返回结点地址
//      如果没有找到,返回NULL
//查找策略: 优先左
//       如果是空树,返回null
//       如果根命中,直接返回根的地址
//       先去左子树中找,如果找到了,返回结果
//       如果左子树也没有找到,再去右子树中找
BNode *Search(BNode *root, TDataType key)
{
	if (root == NULL)
	{
		return 0;
	}
	else if (root->data==key)
	{
		//如果根命中了
		return root;
	}

	BNode *node = Search(root->left, key);
	if (node!=NULL)
	{
		//左子树中找到了
		return node;
	}

	node = Search(root->right, key);
	if (node != NULL)
	{
		return node;
	}

	else
	{
		return NULL;
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~




/*
 QueueInit  初始化队列
 QueueDestroy 销毁队列

 QueuePush    放队列尾
 QueueFront   查看队首结点
 QueuePop     弹出队首结点
 QueueEmpty   判断队列是否为空
*/

#include"Queue.h"
void LevelOrder(BNode *root)
{
	Queue queue;
	QueueInit(&queue);
	if (root == NULL)
	{
		return;
	}

	QueuePush(&queue,root);
	while (!QueueEmpty(&queue))
	{
		BNode *front = QueueFront(&queue);
		QueuePop(&queue);
		printf("%d ",front->data);

		if (front->left != NULL)
		{
			QueuePush(&queue, front->left);
		}
		if (front->right != NULL)
		{
			QueuePush(&queue, front->right);
		}
	}

	QueueDestroy(&queue);
}

//判断二叉树是否是完全二叉树

//带空结点的层序遍历判断是否是完全二叉树
int IsComplete(BNode *root)
{
	Queue queue;
	QueueInit(&queue);
	if (root == NULL)
	{
		return 1;
	}

	QueuePush(&queue, root);
	while (!QueueEmpty(&queue))
	{
		BNode *front = QueueFront(&queue);
		QueuePop(&queue);

		if (front == NULL)
		{
			break;
		}
			QueuePush(&queue, front->left);
			QueuePush(&queue, front->right);
		
	}
	//发现空结点了
	//查看队列中是否完全是NULL
	while (!QueueEmpty(&queue))
	{
		BNode* node = QueueFront(&queue);
		QueuePop(&queue);
		if (node != NULL){
			return 0;
		}
	}

	QueueDestroy(&queue);
	return 1;
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#include"Stack.h"

void PreorderLoop(BNode *root)
{
	Stack stack;
	StackInit(&stack);
	BNode *node = root;
	BNode*top;

	while (node!=NULL || !StackEmpty(&stack)){
		while (node != NULL)
		{
			printf("%d ", node->data);
			StackPush(&stack, node); //压栈,压的是结点地址
			node = node->left;
		}

		top = StackTop(&stack); StackPop(&stack);
		node = top->right;
	}
}

void InorderLoop(BNode *root)
{
	Stack stack;
	StackInit(&stack);
	BNode *node = root;
	BNode*top;

	while (node != NULL || !StackEmpty(&stack)){
		while (node != NULL)
		{	
			StackPush(&stack, node); //压栈,压的是结点地址
			node = node->left;
		}

		top = StackTop(&stack); StackPop(&stack);
		printf("%d ", top->data);
		node = top->right;
	}
}

void PostorderLoop(BNode *root)
{
	Stack stack;
	StackInit(&stack);
	BNode *node = root;
	BNode *top;
	BNode *last = NULL;

	while (node != NULL || !StackEmpty(&stack)){
		while (node != NULL)
		{
			StackPush(&stack, node); //压栈,压的是结点地址
			node = node->left;
		}

		top = StackTop(&stack);
		if (top->right == NULL || top->right == last)
		{
			printf("%d ", top->data);
			last = top;
			StackPop(&stack);
		}
		else{
			node = top->right;
		}
	}
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~`	


void Mirror(BNode *root)
{
	if (root == NULL)
	{
		return;
	}
	BNode *t = root->left;
	root->left = root->right;
	root->right=t;

	Mirror(root->left);
	Mirror(root->right);
}

int IsBlance(BNode *root)
{
	if (root == NULL)
	{
		return 1;
	}

	int isBlance = IsBlance(root->left);
	if (isBlance == 0)
	{
		return 0;
	}

	isBlance = IsBlance(root->right);
	if (isBlance == 0)
	{
		return 0;
	}

	int leftHeight = GetHeight(root->left);
	int rightHeight = GetHeight(root->right);
	int diff = leftHeight - rightHeight;
	if (diff<-1 || diff>1)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

int IsBalance2(BNode *root, int *pHeight)
{
	if (root == NULL)
	{
		//return 高度值
		*pHeight = 0;
		return 1;
	}

	int leftHeight;
	int rightHeight;
	int leftBalance = IsBalance2(root->left, &leftHeight);
	int rightBalance = IsBalance2(root->right, &rightHeight);
	//return 高度值
	*pHeight = MAX(leftHeight, rightHeight) + 1;

	if (leftBalance == 0 || rightBalance == 0)
	{
		return 0;
	}

	int diff = leftHeight - rightHeight;
	if (diff<-1 || diff>1)
	{
		return 0;
	}
	else
	{
		return 1;
	}

}


BNode *GetNearestAncestor(BNode *root, BNode *n1, BNode *n2)
{
	BNode *n1InLeft= Search(root->left, n1->data);
	BNode *n2InLeft = Search(root->left, n2->data);

	if (n1InLeft && !n2InLeft)
	{
		return root;
	}

	if (!n1InLeft && n2InLeft)
	{
		return root;
	}

	if (n1InLeft)
	{
		return GetNearestAncestor(root->left, n1, n2);
	}
	else
	{
		return GetNearestAncestor(root->right, n1, n2);
	}

}

int MAX3(int a, int b, int c)
{
	if (a >= b && a >= c)
	{
		return a;
	}
	 if (b >= a && b >= c)
	{
		return b;
	}
	 return c;
}

int GetFarrestDistance(BNode *root)
{
	if (root == NULL)
	{
		return 0;
	}

	int leftDistance = GetFarrestDistance(root->left);
	int rightDistance = GetFarrestDistance(root->right);

	int leftHeight = GetHeight(root->left);
	int rightHeight = GetHeight(root->right);
	int rootDistance = leftHeight + rightHeight;

	return MAX3(leftDistance, rightDistance, rootDistance);
}

BNode * CreateTree3(TDataType preorder[], TDataType inorder[], int size);


BNode * ReCreatTree(TDataType preorder[], TDataType inorder[], int size)
{
	if (size <= 0)
	{
		return NULL;
	}

	TDataType rootValue = preorder[0];
	int i,r;
	for (i=0; i < size; i++)
	{
		if (inorder[i] == rootValue)
		{
			r = i;
			break;
		}
	}
	assert(i < size);

	BNode *root = CreatNode(rootValue);
	root->left = ReCreatTree(preorder + 1, inorder, r);
	root->right = ReCreatTree(preorder + 1 + r, inorder + r + 1, size - 1 - r);

	return root;
}


void Test()
{
	TDataType preoorder[] = { 1, 2, 4, -1, 3, 5, -1, -1, 3, -1, 6, 7 };
	int size = sizeof(preoorder) / sizeof(TDataType);
	RESULT r = CreateTree(preoorder, size);
	Preorder(r.root);printf("\n");
	PreorderLoop(r.root);printf("\n");

	Inorder(r.root); printf("\n");
	InorderLoop(r.root); printf("\n");

	Postorder(r.root); printf("\n");
	PostorderLoop(r.root); printf("\n");
	

	BNode *root = r.root;
	/*int height;
	int isbalnce = IsBalance2(root, &height);*/
	//height就是树的高度

	

	//printf("结点个数:%d\n", GetSize(root));
	//printf("叶子结点个数:%d\n", GetLeafSize(root));
	//printf("第三层结点个数:%d\n", GetKLevelSize(root, 3));
	//printf("高度:%d\n", GetHeight(root));
	//printf("查找D :%d\n", (Search(root, 4)->data));
	//printf("查找E :%d\n", (Search(root, 5)->data));
	//printf("查找H :%p\n", (Search(root, 8)));

	//LevelOrder(root);
	printf("  %d ", GetFarrestDistance(root));
	//printf("\n")


}