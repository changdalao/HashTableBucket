#pragma once

#include<stdio.h>
#include<malloc.h>
#include<assert.h>
#include<stdlib.h>
#include<string.h>

typedef int KeyType;
typedef int ValueType;

typedef struct HashNode
{
	KeyType _key;
	ValueType _value;
	struct HashNode* _next;
}HashNode;
typedef struct HashTable
{
	HashNode** _table;//定义一个指针数组，存放哈希桶的地址
	size_t size;
	size_t N;
}HashTable;

void HashBucketInit(HashTable* ht);
int HashBucketInsert(HashTable* ht, KeyType key, ValueType value);
void HashBucketPrint(HashTable* ht);
HashNode* BuyHashNode(KeyType key, ValueType value);
int HashBucketRemove(HashTable* ht, KeyType key);
void HashBucketDestrey(HashTable* ht);
HashNode* HashBucketFind(HashTable* ht, KeyType key);

size_t GetNextPrimeNum(size_t num)
{
	const int _PrimeSize = 28;
	static const unsigned long _PrimeList[] =
	{
		53ul,97ul,193ul,389ul,769ul,1543ul,3079ul,
		6151ul,12289ul,24593ul,49157ul,98317ul,196613ul,393241ul,
		786433ul,1572869ul,3145739ul,6291469ul,12582917ul, 25165843ul,
		50331653ul,100663319ul,201326611ul, 402653189ul, 805306457ul,
		1610612741ul, 3221225473ul, 4294967291ul
	};
	for (size_t i = 0; i < _PrimeSize; i++)
	{
		if (_PrimeList[i] > num)
			return _PrimeList[i];
	}
}
int HashFunc(KeyType key,size_t N)
{
	return key % N;
}
void HashBucketInit(HashTable* ht)
{
	assert(ht);
	ht->N = 5;
	ht->size = 0;
	ht->_table = (HashNode**)malloc(sizeof(HashNode*)*ht->N);
	memset(ht->_table, NULL, sizeof(HashNode*)*ht->N);
	assert(ht->_table);
}
HashNode* BuyHashNode(KeyType key, ValueType value)
{
	HashNode* NewHade = (HashNode*)malloc(sizeof(HashNode));
	assert(NewHade);
	(NewHade)->_key = key;
	(NewHade)->_value = value;
	(NewHade)->_next = NULL;
	return NewHade;
}
void HashBucketPrint(HashTable* ht)
{
	assert(ht);
	size_t i = 0;
	for (; i < ht->N; i++)
	{
		printf("[table%d]",i);
		HashNode* cur = ht->_table[i];
		if (cur != NULL)
		{
			while (cur)
			{
				printf("->%d", cur->_key);
				cur = cur->_next;
			}
		}
		printf("->NULL\n");
	}
}
HashNode* HashBucketFind(HashTable* ht, KeyType key)
{
	assert(ht);
	size_t index = HashFunc(key, ht->N);
	if (ht->_table[index] == NULL)
		return NULL;
	else
	{
		if (ht->_table[index]->_key == key)
			return ht->_table[index];
		else
		{
			HashNode* cur = ht->_table[index]->_next;
			while (cur)
			{
				if (cur->_key == key)
					return cur;
				cur = cur->_next;
			}
			return NULL;
		}
	}
}
int HashBucketInsert(HashTable* ht, KeyType key, ValueType value)
{
	assert(ht);
	if (ht->size == ht->N)//扩容
	{
		size_t NewN = GetNextPrimeNum(ht->N);
		HashNode** Newtable = (HashNode**)malloc(sizeof(HashNode*)*NewN);
		assert(Newtable);
		memset(Newtable, NULL, sizeof(HashNode*)*NewN);//新表初始化
		for (size_t i=0; i < ht->N; i++)
		{
			if (ht->_table[i] != NULL)
			{
				HashNode* cur = ht->_table[i];
				while (cur)
				{
					HashNode* next = cur->_next;
					size_t NewIndex = HashFunc(cur->_key, NewN);
					cur->_next = Newtable[NewIndex];
					Newtable[NewIndex] = cur;
					cur = next;
				}
			}
			else
				continue;
		}
		free(ht->_table);
		ht->_table = Newtable;
		ht->N = NewN;
		//free(Newtable);
	}
	size_t index = HashFunc(key, ht->N);
	HashNode* cur = ht->_table[index];
	HashNode* NewNode = BuyHashNode(key, value);
	//index位置上无数据   index位置上有数据，进行头插
	if (ht->_table[index] == NULL)
		ht->_table[index] = NewNode;
	else
	{
		while (cur)//判断链表里有没有相同的元素，有则直接返回-1
		{
			if (cur->_key == NewNode->_key)
				return -1;
			cur = cur->_next;
		}
		//头插
		NewNode->_next = ht->_table[index];
		ht->_table[index] = NewNode;
	}
	ht->size++;
	return 0;
}
int HashBucketRemove(HashTable* ht, KeyType key)
{
	assert(ht);
	//链表只有一个数据，即头
	//链表一堆数据，删其中任何一个
	size_t index = HashFunc(key, ht->N);
	if (ht->_table[index] != NULL)
	{
		HashNode* cur = ht->_table[index];
		HashNode* parent = ht->_table[index];
		if (ht->_table[index]->_key == key)//删头
		{
			ht->_table[index] = cur->_next;
			free(cur);
			cur = NULL;
			return 0;
		}
		else
		{
			while (cur)
			{
				if (cur->_key == key)
				{
					parent->_next = cur->_next;
					free(cur);
					cur = NULL;
					return 0;
				}
				parent = cur;
				cur = cur->_next;
			}
		}
		return -1;
	}
}
void HashBucketDestrey(HashTable* ht)
{
	assert(ht);
	ht->size = 0;
	ht->N = 0;
	free(ht->_table);
}

void HashBucketTest()
{
	HashTable ht;
	HashBucketInit(&ht);
	HashBucketInsert(&ht, 5, 0);
	HashBucketInsert(&ht, 58, 0);
	HashBucketInsert(&ht, 111, 0);
	HashBucketInsert(&ht, 1, 0);
	HashBucketInsert(&ht, 54, 0);
	HashBucketInsert(&ht, 25, 0);
	HashBucketInsert(&ht, 85, 0);
	HashBucketInsert(&ht, 0, 0);
	HashBucketInsert(&ht, 4, 0);
	HashBucketInsert(&ht, 78, 0);
	HashBucketInsert(&ht, 52, 0);
	HashBucketInsert(&ht, 105, 0);
	HashBucketPrint(&ht);
	printf("查找5->%d\n", HashBucketFind(&ht, 5)->_key);
	printf("查找111->%d\n", HashBucketFind(&ht, 111)->_key);
	printf("查找0->%d\n", HashBucketFind(&ht, 0)->_key);
	printf("查找1->%d\n", HashBucketFind(&ht, 1)->_key);
	printf("/////////////////////////////////////////////////////\n");
	HashBucketRemove(&ht, 5);
	HashBucketRemove(&ht, 58);
	HashBucketRemove(&ht, 111);
	HashBucketRemove(&ht, 4);
	HashBucketPrint(&ht);

	HashBucketDestrey(&ht);
	HashBucketPrint(&ht);
}