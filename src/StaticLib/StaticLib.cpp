﻿#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
#include "Windows.h"                    // Windows API の機能定義
#include <stdlib.h>

#include "../include/lib_func.h"

static unsigned int hash_func(unsigned int key, int num)
{
	return key % num;
}

static unsigned int get_hash(const hash* h, unsigned int key)
{
	if (h == NULL) return ~0;
	if (key == ~0) return ~0;

	// ToDo: ハッシュ関数としてhash_funcを使った
	// オープンアドレス法によるハッシュ値を求める

	unsigned int n = hash_func(key, h->max_size);

	while (n < h->max_size) 
	{
		if (h->nodes[n].key == ~0 || h->nodes[n].key == key)
		{
			return n;
		}

		n++;
	}
	return ~0;
}

// ハッシュの初期化(max_sizeは~0未満)
bool initialize(hash* h, unsigned int max_size)
{
	if (h == NULL) return false;
	if (max_size == 0 || max_size == ~0) return false;

	if (h->nodes != NULL) finalize(h);

	h->nodes = (hash_node*)malloc(max_size * sizeof(hash_node));
	if (NULL == h->nodes) return false;

	for (unsigned int i = 0; i < max_size; i++) {
		h->nodes[i].key = ~0;
		h->nodes[i].value[0] = '\0';
	}

	h->max_size = max_size;

	return true;
}

// 使用メモリの全解放
void finalize(hash* h)
{
	if (h == NULL) return;

	free(h->nodes);
	h->nodes = NULL;
	h->max_size = 0;
}

// keyの値を見てノードを追加する(追加できなかったらfalseを返す)
bool add(hash* h, unsigned int key, const char* value)
{
	if (h == NULL) return false;
	if (h->max_size == ~0) return false;
	if (key == ~0) return NULL;

	// ToDo: ハッシュ関数としてhash_funcを使った
	// オープンアドレス法によりキーを追加

	unsigned int n = get_hash(h, key);

	if (n != ~0) 
	{
		h->nodes[n].key = key;

		for (int i = 0; i < 256; i++) 
		{
			h->nodes[n].value[i] = value[i];
		}
		return true;
	}

	return false;
}

// keyの値を見てノードを検索して、値を取得する(なければNULLを返す)
const char* get(const hash* h, unsigned int key)
{
	if (key == ~0) return NULL;

	// ToDo: keyから値が格納されている場所を求め、値の場所を返す
	for (int i = 0; i < h->max_size; i++) 
	{
		if (h->nodes[i].key == key) 
		{
			return h->nodes[i].value;
		}
	}
	return NULL;
}

// ハッシュの値を取得する(デバッグ用)
int debug_get_hash(const hash* h, unsigned int key)
{
	return get_hash(h, key);
}