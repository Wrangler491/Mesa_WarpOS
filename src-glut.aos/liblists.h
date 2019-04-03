/*
 *  common functions to mange double-linked-lists
 *  Copyright (C) 1997-8  Niels Froehling <Niels.Froehling@Informatik.Uni-Oldenburg.DE>
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, write to the Free
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef LISTS_H
#define LISTS_H

struct nnode {
  struct nnode *succ;						/* Pointer to next (successor) */
  struct nnode *pred;						/* Pointer to previous (predecessor) */
  int data;
};

struct nlist {
  struct nnode *head;
  struct nnode *tail;
  struct nnode *tailpred;
  int nodes;
};

extern void nNewList(register struct nlist *list);
void nAddHead(register struct nlist *list, register struct nnode *node);
extern void nAddTail(register struct nlist *list, register struct nnode *node);
struct nnode *nRemHead(register struct nlist *list);
struct nnode *nRemTail(register struct nlist *list);
void nRemove(register struct nlist *list, register struct nnode *node);
void nInsert(register struct nlist *list, register struct nnode *node,
	     register struct nnode *pred);
void npreInsert(register struct nlist *list, register struct nnode *node,
		register struct nnode *succ);
struct nnode *nGetHead(register struct nlist *list);
struct nnode *nGetTail(register struct nlist *list);
struct nnode *nSuccNode(register struct nnode *node);
void nMoveList(register struct nlist *slist, register struct nlist *dlist);
void nAppendList(register struct nlist *preList,
		 register struct nlist *postList);

/* from higher values to lower */
struct nnode *nFindNode(register struct nnode *startnode,
			register int numnodes, register int data);
void nEnqueue(register struct nlist *list, register struct nnode *node);
void nMoveNodeSorted(register struct nlist *slist, register struct nlist *dlist,
		     register struct nnode *node);
void nSortListWeighted(register struct nlist *list, register int radix);
void nSortListLinear(register struct nlist *list, register int radix);
void nSortListLinearMax(register struct nlist *list, register int radix,
			register int radix_divide);

/* from lower values to higher */
struct nnode *nFindNodeReverse(register struct nnode *startnode,
			       register int numnodes, register int data);
void nEnqueueReverse(register struct nlist *list, register struct nnode *node);
void nMoveNodeSortedReverse(register struct nlist *slist, register struct nlist *dlist,
			    register struct nnode *node);
void nSortListWeightedReverse(register struct nlist *list, register int radix);
void nSortListLinearReverse(register struct nlist *list, register int radix);
void nSortListLinearMaxReverse(register struct nlist *list, register int radix,
			       register int radix_divide);

#define	nGetNext(node)		nSuccNode(node)

#endif								/* LISTS_H */
