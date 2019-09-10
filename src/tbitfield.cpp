// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"

TBitField::TBitField(int len)
{
	if (len > 0)
	{
		BitLen = (((len - 1) >> 3) + 1) << 3;
		MemLen = len;
		pMem = new TELEM[BitLen];
		for (int i = 0; i < MemLen; i++)
			pMem[i] = 0;
	}
	else throw exception("wrong number");
}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;
	pMem = new TELEM[BitLen];
	if (pMem != 0)
	{
		for (int i = 0; i < MemLen; i++)
			pMem[i] = bf.pMem[i];
		for (int i = MemLen; i < BitLen; i++)
			pMem[i] = 0;
	}
}

TBitField::~TBitField()
{
	BitLen = 0;
	MemLen = 0;
	if (pMem != 0) delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n ???
{
	return n >> ((BitLen >> 2) + 1);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	return 1 << (n & (BitLen - 1));
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return MemLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n > -1) && (n < MemLen))
	{
		pMem[GetMemMask(n)] = pMem[GetMemMask(n)] || GetMemMask(n);
	}
	else if (n < 0) throw exception("Negative index");
	else throw exception("Too large index");
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n > -1) && (n < MemLen))
	{
		pMem[GetMemMask(n)] = pMem[GetMemMask(n)] && (!GetMemMask(n));
	}
	else if (n < 0) throw exception("Negative index");
	else throw exception("Too large index");
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n > -1) && (n < MemLen))
	{
		return pMem[GetMemMask(n)] && GetMemMask(n);
	}
	else if (n < 0) throw exception("Negative index");
	else throw exception("Too large index");
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
	if (this != &bf)
	{
		if (BitLen != bf.BitLen)
		{
			BitLen = bf.BitLen;
			pMem = new TELEM[BitLen];
		}

		MemLen = bf.MemLen;

		for (int i = 0; i < MemLen; i++)
			pMem[GetMemMask(i)] = bf.pMem[GetMemMask(i)];
		for (int i = MemLen; i < BitLen; i++)
			pMem[GetMemMask(i)] = 0;
	}

	return *this;
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
	if (MemLen != bf.MemLen) return 0;
	else
	{
		int f = 0;
		for (int i = 0; i < MemLen; i++)
			if (pMem[GetMemMask(i)] != bf.pMem[GetMemMask(i)])
			{
				f = 1;
				break;
			}
		if (f) return 0;
		else return 1;
	}
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
	if (MemLen != bf.MemLen) return 1;
	else
	{
		int f = 0;
		for (int i = 0; i < MemLen; i++)
			if (pMem[i] == bf.pMem[i])
			{
				f = 1;
				break;
			}
		if (f) return 0;
		else return 1;
	}
}

TBitField TBitField::operator|(const TBitField &bf) // операция "или"
{
	int ResMem, MinMem;
	if (MemLen >= bf.MemLen)
	{
		ResMem = MemLen;
		MinMem = bf.MemLen;

		TBitField Result(ResMem);

		for (int i = 0; i < MinMem; i++)
			Result.pMem[GetMemMask(i)] = pMem[GetMemMask(i)] || bf.pMem[GetMemMask(i)];

		for (int i = MinMem; i < ResMem; i++)
			Result.pMem[GetMemMask(i)] = pMem[GetMemMask(i)];

		for (int i = ResMem; i < Result.BitLen; i++)
			Result.pMem[GetMemMask(i)] = 0;

		return Result;
	}
	else
	{
		ResMem = bf.MemLen;
		MinMem = MemLen;

		TBitField Result(ResMem);

		for (int i = 0; i < MinMem; i++)
			Result.pMem[GetMemMask(i)] = pMem[GetMemMask(i)] || bf.pMem[GetMemMask(i)];

		for (int i = MinMem; i < ResMem; i++)
			Result.pMem[GetMemMask(i)] = bf.pMem[GetMemMask(i)];

		for (int i = ResMem; i < Result.BitLen; i++)
			Result.pMem[GetMemMask(i)] = 0;

		return Result;
	}
}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
	int ResMem, MinMem;
	if (MemLen >= bf.MemLen)
	{
		ResMem = MemLen;
		MinMem = bf.MemLen;

		TBitField Result(ResMem);

		for (int i = 0; i < MinMem; i++)
			Result.pMem[i] = pMem[i] && bf.pMem[i];

		for (int i = MinMem; i < Result.BitLen; i++)
			Result.pMem[i] = 0;

		return Result;
	}
	else
	{
		ResMem = bf.MemLen;
		MinMem = MemLen;

		TBitField Result(ResMem);

		for (int i = 0; i < MinMem; i++)
			Result.pMem[i] = pMem[i] && bf.pMem[i];

		for (int i = MinMem; i < Result.BitLen; i++)
			Result.pMem[i] = 0;

		return Result;
	}
}

TBitField TBitField::operator~(void) // отрицание
{
	for (int i = 0; i < BitLen; i++)
		pMem[i] = !pMem[i];
	return *this;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
	cout << "Введите битовое поле, начиная со старшего бита\n";
	for (TELEM i = bf.MemLen; i >= 0; i--)
		cin >> bf.pMem[i];

	return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{
	cout << "\n";
	for (int i = bf.MemLen; i > -1; i--)
		cout << bf.pMem[i];
	cout << "\n";

	return ostr;
}
