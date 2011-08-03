#ifndef BST_H_
#define BST_H_


template< typename Type >
class BST
{

private:

	struct Node
	{
		Node* Left;
		Node* Right;
		int nTrueID;
		Type element;

	public:

		Node&
			operator=( Node& input )
		{

			Left  = input.Left;
			Right = input.Right;
			nTrueID = input.nTrueID;
			element = input.element;

		};

	};

	Node* Root;


public:

	/////////////////////////////////////////////////////////////////////////////
	// Function : Constuctor
	// Notes : constucts an empty BST
	/////////////////////////////////////////////////////////////////////////////
	BST() : Root( nullptr ) {};

	/////////////////////////////////////////////////////////////////////////////
	// Function : Destructor
	// Notes : destroys the BST cleaning up any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	~BST();

	/////////////////////////////////////////////////////////////////////////////
	// Function : assignment operator
	/////////////////////////////////////////////////////////////////////////////
	BST& operator=(const BST& that);

	/////////////////////////////////////////////////////////////////////////////
	// Function: copy constructor
	/////////////////////////////////////////////////////////////////////////////
	BST(const BST& that);

	/////////////////////////////////////////////////////////////////////////////
	// Function : insert
	// Parameters :  v - the item to insert 
	/////////////////////////////////////////////////////////////////////////////
	void insert(const Type& v);

	/////////////////////////////////////////////////////////////////////////////
	// Function : findAndRemove
	// Parameters : v - the item to find (and remove if it is found)
	// Return : bool - true if the item was removed, false otherwise
	/////////////////////////////////////////////////////////////////////////////
	bool findAndRemove(const Type& v);

	/////////////////////////////////////////////////////////////////////////////
	// Function : find
	// Parameters : v - the item to find
	// Return : bool - true if the item was found, false otherwise
	/////////////////////////////////////////////////////////////////////////////
	bool find(const Type& v) const;

	/////////////////////////////////////////////////////////////////////////////
	// Function : clear
	// Notes : clears the BST, cleaning up any dynamic memory
	/////////////////////////////////////////////////////////////////////////////
	void clear();

	/////////////////////////////////////////////////////////////////////////////
	// Function : printInOrder
	// Notes : prints the contents of the BST to the screen, in ascending order
	/////////////////////////////////////////////////////////////////////////////
	void printInOrder() const;

	void rPrint( Node* input ) const;

	void 
		rClear( Node* input ); 

	Node* 
		rCopy( Node* input );

	void
		rInsert( Node* input, const Type& v );

	bool rFind( Node* input, const Type& v ) const;

	void InsertEntry( const Type& _searchKey, int _trueValue );
	void rInsertEntry( Node* input, const Type& _searchKey, int _trueValue );

	int ConvertTrueValue( const Type& _searchKey );
	int rConvertTrueValue( Node* input, const Type& _searchKey );


	void DecrementFromNode( int _testValue );
	void rDecrementFromNode( Node* input, int _testValue );

	bool NewFindandRemove( const Type& v );

};


template< typename Type >
BST<Type>::~BST()
{
	rClear( Root );
}

template< typename Type >
BST<Type>& 
	BST<Type>::operator=( const BST& that )
{
	if( this != &that )
	{

		rClear( Root );

		if( that.Root )
		{
			Root = rCopy( that.Root );
		}
	}

	return *this;
}

template< typename Type >
BST<Type>::BST(const BST& that)
{

	if( that.Root )
	{
		Root = rCopy( that.Root );
	}
	else
		Root = nullptr;



}

template< typename Type >
void
	BST<Type>::insert(const Type& v)
{
	if( !Root )
	{
		Root = new Node;
		Root->element = v;
		Root->Left = nullptr;
		Root->Right = nullptr;

	}
	else
		rInsert( Root, v );

}

template< typename Type >
bool
	BST<Type>::findAndRemove(const Type& v)
{
	Node* Curr = Root;
	Node* Prnt = nullptr;
	Node* temp = nullptr;

	for( ; Curr != nullptr ; )
	{
		if( v < Curr->element )
		{
			Prnt = Curr;
			Curr = Curr->Left;
		}
		else if( v > Curr->element )
		{
			Prnt = Curr;
			Curr = Curr->Right;
		}
		else
			break;
	}

		if( Curr == nullptr )
			return false;

		if( Curr->Left && Curr->Right )
		{
			temp = Curr->Right;
			while( temp->Left != nullptr )
			{
				temp = temp->Left ;
			}

			Curr->element = temp->element;
			Curr = temp ;
		}
		else if( !Curr->Left && !Curr->Right )
		{
			if( Curr == Root )
			{
				Root = nullptr; 
			}
			else
			{
				if( Prnt)
				{
					if( Curr == Prnt->Left )
					{
						Prnt->Left = nullptr;
					}
					else
					{
						Prnt->Right = nullptr;
					}
				}
			}

			delete Curr;
		}
		else if( Curr->Right || Curr->Left )
		{

			if( Curr == Root )
			{

				if( Root->Left )
				{
					Root = Curr->Left;
				}
				else
				{
					Root = Curr->Right;
				}

			}
			else
			{
				if( Prnt )
				{
					if( Curr == Prnt->Left )
					{
						if( Curr->Left)
						{
							Prnt->Left = Curr->Left;
						}
						else
						{
							Prnt->Left = Curr->Right;
						}
					}
					else if( Curr == Prnt->Right )
					{
						if( Curr->Left)
						{
							Prnt->Right = Curr->Left;
						}
						else
						{
							Prnt->Right = Curr->Right;
						}

					}
				}
			}

			delete Curr;
		}

	return true;
}

template< typename Type >
bool 
	BST<Type>::find(const Type& v) const
{	
	return rFind( Root, v );
}

template< typename Type >
void
	BST<Type>::clear()
{
	rClear( Root );
	Root = nullptr;
}

template< typename Type >
void
	BST<Type>::printInOrder()
	const
{

	rPrint( Root );
	cout << endl;

}

template< typename Type >
void
	BST<Type>::rClear( Node* input )
{
	if( input )
	{
		if( input->Left )
			rClear( input->Left );
		if( input->Right )
			rClear( input->Right );

		delete input;
	}
}

template< typename Type >
typename BST<Type>::Node* 
	BST<Type>::rCopy( Node* input )
{
	if( input )
	{
		Node* temp = new Node;
		temp->element = input->element;
		temp->Left = nullptr;
		temp->Right = nullptr;

		if( input->Left )
			temp->Left = rCopy( input->Left );


		if( input->Right )
			temp->Right = rCopy( input->Right);


		return temp;
	}
	else
		return nullptr;

}

template< typename Type >
void
	BST<Type>::rInsert( Node* input, const Type& v )
{

	if( input != nullptr )
	{

		if( !Root )
			Root = input;

		if( v < input->element )
		{
			if( input->Left == nullptr)
			{
				input->Left = new Node;
				input->Left->element = v;
				input->Left->Left = nullptr;
				input->Left->Right = nullptr;
			}
			else
				rInsert( input->Left, v );
		}
		else if( v >= input->element )
		{
			if( input->Right == nullptr)
			{
				input->Right = new Node;
				input->Right->element = v;
				input->Right->Left = nullptr;
				input->Right->Right = nullptr;
			}
			else
				rInsert( input->Right, v );
		}
	}
}

template< typename Type>
void
	BST<Type>::rPrint( Node* input ) const
{
	if( input )
	{
		if( input->Left )
			rPrint( input->Left );

		cout << input->element << " ";

		if( input->Right )
			rPrint( input->Right );
	}	
}

template< typename Type >
bool
	BST<Type>::rFind( Node* input, const Type& v ) const
{
	if( input )
	{
		if( v == input->element)
			return 1;
		if( v < input->element && input->Left )
			return rFind( input->Left, v );
		else if( v >= input->element && input->Right )
			return rFind( input->Right, v );
		else
			return 0;
	}
	else
		return 0;
}

template< typename Type >
void 
	BST<Type>::InsertEntry( const Type& _searchKey, int _trueValue )
{
	if( !Root )
	{
		Root = new Node();
		Root->element = _searchKey;
		Root->nTrueID = _trueValue;
		Root->Left = nullptr;
		Root->Right = nullptr;

	}
	else
		rInsertEntry( Root, _searchKey, _trueValue );
}

template< typename Type >
void 
	BST<Type>::rInsertEntry( Node* input, const Type& _searchKey, int _trueValue )
{
	if( input != nullptr )
	{

		if( !Root )
			Root = input;

		if( _searchKey == input->element )
		{
			input->nTrueID = _trueValue;
		}
		else if( _searchKey < input->element )
		{
			if( input->Left == nullptr)
			{
				input->Left = new Node();
				input->Left->element = _searchKey;
				input->Left->nTrueID = _trueValue;
				input->Left->Left = nullptr;
				input->Left->Right = nullptr;
			}
			else
				rInsertEntry( input->Left, _searchKey, _trueValue );
		}
		else if( _searchKey >= input->element )
		{
			if( input->Right == nullptr)
			{
				input->Right = new Node;
				input->Right->element = _searchKey;
				input->Right->nTrueID = _trueValue;
				input->Right->Left = nullptr;
				input->Right->Right = nullptr;
			}
			else
				rInsertEntry( input->Right, _searchKey, _trueValue );
		}
	}
}

template< typename Type >
int 
	BST<Type>::ConvertTrueValue( const Type& _searchKey )
{
	return rConvertTrueValue( Root, _searchKey );
}

template< typename Type >
int 
	BST<Type>::rConvertTrueValue( Node* input, const Type& _searchKey )
{
	if( input && input != (void*)0xfeeefeee  )
	{
		if( _searchKey == input->element)
			return input->nTrueID;
		if( _searchKey < input->element && input->Left )
			return rConvertTrueValue( input->Left, _searchKey );
		else if( _searchKey >= input->element && input->Right )
			return rConvertTrueValue( input->Right, _searchKey );
		else
			return -1;
	}
	else
		return -1;
}


template< typename Type >
void 
	BST<Type>::DecrementFromNode( int _testValue )
{
	rDecrementFromNode( Root, _testValue );

}

template< typename Type >
void 
	BST<Type>::rDecrementFromNode( Node* input, int _testValue )
{
	if( input )
	{
		if( input->nTrueID > _testValue )
			input->nTrueID -= 1;

		rDecrementFromNode( input->Left, _testValue );
		rDecrementFromNode( input->Right, _testValue );
	}
}

template< typename Type >
bool 
	BST<Type>::NewFindandRemove( const Type& v )
{
	Node* Curr = Root;
	Node* Prnt = nullptr;
	Node* temp = nullptr;
	return false;
}

#endif