#include<cassert>
#include<limits>  //for numeric_limits
#include<cstddef> //std::size_t
#include<fstream>
#include<iostream> 
#include<iomanip> //for setw()

typedef float Element_type;

class Matrix
{
    private:
	size_t mColCount;
	size_t mRowCount;
	Element_type * mData;
	
    public:
	void initialize( const std::size_t r, const std::size_t c )
	{
		mRowCount = r;
		mColCount = c;
		mData = new Element_type[r*c];
	}

	void setElement( const std::size_t i, const std::size_t j,const  Element_type  e)
	{
		mData[i* mColCount + j] = e;
	}
	
	Element_type & getElement( const std::size_t i , const std::size_t j) const
	{
		return mData[ i * mColCount + j ];
	}

	size_t getCol() const
	{
		return mColCount;
	}

	size_t getRow() const
	{
		return mRowCount;
	}
};

void readMatrix( Matrix & matrix, std::ifstream & like_pipe )
	{
	assert( like_pipe.is_open() );
	std::size_t r= std::numeric_limits<std::size_t>::max();
	like_pipe >> r;
	assert(r < std::numeric_limits<Element_type>::max() );
	assert(r > 0);
	assert( like_pipe.is_open() );
	std::size_t c = std::numeric_limits<std::size_t>::max();
	like_pipe >> c;
	assert(c < std::numeric_limits<Element_type>::max() );
	assert(c > 0);
	Element_type x = std::numeric_limits<Element_type>::max();
	matrix.initialize(r,c);
//	Element_type y = sizeof(matrix)/sizeof(matrix.getElement(0,0));//under construction
	//r = y * matrix.getCol();
	//std::cout << y << std::endl;
	//c = 
	for( std::size_t i = 0; i < r; ++i)
	{
		for( std::size_t j=0; j< c; ++j)
		{
			like_pipe>> x; //OK version
			assert( x < std::numeric_limits<Element_type>::max());
			matrix.setElement(i,j,x);
		}//end for(j;j;j)
			std::cout << std::endl;
	}//end for(i;i;i)
}

Element_type  myRound( Element_type  varm)
{
	Element_type value = (int)(varm * 10000);
	return (Element_type)value/10000;
}


void myRound_1(Element_type varm, Element_type result)
{
	Element_type value = ( int )( varm * 10000);
		result = (Element_type)result/10000;
		std::cout << result; 
}


Element_type   myAbs( Element_type   var)
{
	if( var > 0) 
       	{
		return var;
	}
	else
	{
		return  (-1)* var;
	}
}

void writeMatrix(const Matrix & matrix , std::ofstream & like_pipe)
{
	assert(like_pipe.good());
	like_pipe<< matrix.getRow() << " ";
	assert(like_pipe.good());
	like_pipe<< matrix.getCol() << "\n";
	for(std::size_t i = 0; i < matrix.getRow(); ++i)
	{
		for( std::size_t j = 0; j< matrix.getCol(); ++j)
		{
			like_pipe<< std::setw(10) <<  matrix.getElement(i,j)<< " "; //OK version
		}//end for(j;j;j)
		like_pipe << std::endl;
	}//end for(i;i;i)	
}

void showMatrix( Matrix & x)
{
	std::cout << "++++++++++++++++++++++++test+++++++++++++++++++++++++++++++++++++\n";
	for(std::size_t i = 0; i < x.getRow(); ++i)
	{
		for(std::size_t j = 0; j < x.getCol(); ++j)
		{
		       	std::cout << std::setw(9) << x.getElement(i,j) << " ";
		}//end for(j;j;j)
		std::cout << std::endl;
	}//end for(i;i;i)

}

void supplementStep(const  Matrix & a, const  Matrix &b, Matrix &x)
{
	for( std::size_t i = 0; i < a.getRow(); ++i)
	{
		for( std::size_t j = 0; j <= a.getCol(); ++j)
		{
		Element_type temp_a = a.getElement( i,j);
		x.setElement(i,j, temp_a);
	//	std::cout << x.getElement(i,j) << " ";
		}//end for(j;j;j)
		Element_type temp_b = b.getElement(i,0);
		x.setElement(i, a.getCol(), temp_b);
	//	std::cout <<x.getElement(i,6 ) << std::endl ;
	}//end for(i;i;i)
}


void findMaxModuleElement(const Matrix & x, std::size_t & column_index ) //number of colum with abs max value
{
	std::size_t row= x.getRow() - 1;
	Element_type maxModule = 0;
	maxModule = x.getElement(row,0);
	for( std::size_t j = 1; j < x.getCol(); ++j)
	{
		if( myAbs(maxModule) <  myAbs(x.getElement( row,j)))
		{
		   maxModule  = x.getElement(row, j);
			column_index = j;
		}
		else 
		{
			continue;
		}//end else
	}//end for(j;j;j)	
//	std::cout << column_index<< std::endl;//test
//	std::cout << maxModule <<std::endl;//test
}

void divideFreeMember(Matrix  x_before, std::size_t & column_index, std::size_t & row_index)
{
	//std::cout << column_index  << std::endl; //test
	std::size_t column = x_before.getCol() -1;
	Element_type temp1 = x_before.getElement(0, column ) / x_before.getElement(0,column_index);
	for(std::size_t i = 1; i< x_before.getRow(); ++i)
	{
		Element_type temp2 = x_before.getElement(i, column ) / x_before.getElement(i,column_index);	
		if( x_before.getElement(i,column)  == 0 ) { temp2 = 0 ;}; // avoiding -0 result
		
		if(temp2 != 0)
		{
			if(temp1 > temp2 )
			{
				row_index = i;
				temp1 = temp2;
			}//end if
			else 
			{
			continue;	
			}//end else
		}//end if
		else 
		{
			continue;
		}
	}//end for
	//	std::cout << "temp2 = " << temp1 << " index = " << column_index << std::endl;//test
}


void findMinColumnElement(Matrix & x, std::size_t&  row_index)
{
	std::size_t column = x.getCol()-1;
	Element_type value  = x.getElement(0,column); //; = x.getElement(0,column );
	for( std::size_t i = 0; i < x.getRow(); ++i)
	{
		if( value < x.getElement(i,column) )
		{
			continue;
		}//end if
		else
		{
			if( x.getElement(i, column) != 0 )
			{	
			value = x.getElement(i,column);
			row_index = i;
			}//end if
		}//end else
	}//end for(i;i;i)
//	std::cout <<"\n " <<  value << " " << row_index << std::endl; //test
}

void dividePivotColumn( Matrix & x,  std::size_t & row , std::size_t & col)
{
	Element_type pivot = x.getElement(row,col);
	Element_type temp =0;
	for( std::size_t j=0; j< x.getCol() ; ++j  )
	{
		temp =x.getElement(row, j) / pivot;
		x.setElement(row,j,temp );
	}//end for
}

void substractPivotRow( Matrix & x, std::size_t & index_row, std::size_t & index_column)
{
	Element_type temp = 0;
	Element_type coef = 0;
	for(std::size_t i =0; i < x.getRow(); ++i)
	{
		coef = x.getElement(i ,index_column);  // x.getElement( i,j);
		for( std::size_t j = 0; j < x.getCol(); ++j)
		{
			if(i != index_row)
			{
			   temp = x.getElement(i,j)- x.getElement(index_row,j)*coef;
	                   if( temp == -0)
			   {
				   temp = 0;
			   }
			   else
			   {
				  temp = temp;
			   }
			   x.setElement(i,j,temp);
			}//end if
			else
			{ continue;}
		}//for(j;j;j)
	}//end for(i;i;i)
}


void simplexMethod(const  Matrix & a, const  Matrix &b, Matrix &x)
{
	std::size_t index_column = 0;
	std::size_t index_row = 0;
	supplementStep(a,b,x);                         //...DONE
	
//	if()
	showMatrix(x);
	findMaxModuleElement(x, index_column);          //..DONE
	showMatrix(x);
	divideFreeMember( x,index_column,  index_row);  //..DONE 
	showMatrix(x);
	dividePivotColumn(x, index_row, index_column);  //..DONE
	showMatrix(x);
	substractPivotRow(x, index_row, index_column);  //..DONE
	showMatrix(x);
	

	
	//	std::cout << "index_col = " << index_column << " \n index_row = " << index_row << "\n";//test
}

int main()
{
	Matrix a;
	Matrix b;
	Matrix x;
	std::ifstream A("A.txt"), B("B.txt");
	std::ofstream X("x.txt");
	readMatrix(a,A); 
	readMatrix(b,B);
	x.initialize(a.getRow() ,a.getCol() + 1);
	simplexMethod( a, b, x);
	writeMatrix(x, X);
	return 0;
}
