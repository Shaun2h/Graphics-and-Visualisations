#include "MatrixStack.h"
#include <cstdlib>
#include <iostream>

MatrixStack::MatrixStack()
{
	// Initialize the matrix stack with the identity matrix.
	m_matrices.push_back(Matrix4f::identity()); //start with identity matrix
}

void MatrixStack::clear()
{
	m_matrices.clear();
	m_matrices.push_back(Matrix4f::identity());
	// Revert to just containing the identity matrix.
}

Matrix4f MatrixStack::top()
{
	
	// Return the top of the stack
	return m_matrices.back();
}

void MatrixStack::push( const Matrix4f& m )
{
	// Push m onto the stack.
	// Your stack should have OpenGL semantics:
	// the new top should be the old top multiplied by m
	Matrix4f a = m_matrices.back(); //grab old
	Matrix4f newbie = a * m;//newdude
	m_matrices.push_back(newbie); 
}

void MatrixStack::pop()
{
	m_matrices.pop_back(); //pop the back
	// Remove the top element from the stack
}


/*
 HEADER HEADER HEADER HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER 
 HEADER HEADER HEADER HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER  HEADER HEADER HEADER 



#ifndef MATRIX_STACK_H
#define MATRIX_STACK_H

#include <vector>
#include <vecmath.h>

class MatrixStack
{
public:
	MatrixStack();
	void clear();
	Matrix4f top();
	void push( const Matrix4f& m );
	void pop();

private:

	std::vector< Matrix4f > m_matrices;

};

#endif // MATRIX_STACK_H

*/