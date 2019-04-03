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

//#include "../include/init.h"

#if !(defined(HAVE_LIBLISTS) || defined(LISTS_C))
#define LISTS_C

/* Name : list.c
 *
 * Notes: The following passage is taken verbatim from _Amiga_ROM_Kernal
 *        _Reference_Manual:_Libraries_3rd_Edition_, page 490
 *
 * One subtlety here must be explained further.  The list header is
 * constructed in an efficient, but confusing manner.  This of the header
 * as a structure containg the head and tail nodes for the list.  The
 * head and tail nodes are placeholders, and never carry data.  The head
 * and tail portions of the header actually overlap in memory.  lh_Head and
 * lh_Tail form the head node; lh_Tail and lh_TailPred form the tail node.
 * This makes it easy to find the start or end of the list, and eliminates
 * any special cases for insertion or removal.
 *
 *   "Head Node"   "Tail Node"         "Merged Header"
 * +-------------+                     +-------------+
 * |   ln_Succ   |                     |   lh_Head   |
 * +-------------+-------------+       +-------------+
 * | ln_Pred = 0 | ln_Succ = 0 |  ---> | lh_Tail = 0 |
 * +-------------+-------------+       +-------------+
 *               |   ln_Pred   |       | ln_TailPred |
 *               +-------------+       +-------------+
 *
 *           Figure 23-2: List Header Overlap
 *
 * The lh_Head and lh_Tail fields of the list header act like the
 * ln_Succ and lh_Pred fields of a node.  The lh_Tail field is set
 * permanently to NULL, indicating that the head node is indeed the
 * first on the list -- that is, it has no predecessors.
 *
 * Likewise, the lh_Tail and lh_TailPred fields of the list header
 * act like the ln_Succ and ln_Pred fields of a node.  Here the NULL
 * lh_Tail indicates that the tail node is indeed the list on the
 * list -- that is, it has no successors.
 *
 *      $Log:   list.c,v $
 * Revision 2.0  98/07/09  04:57:56  nf
 * Completely rewritten, only the comments left for documentation
 * added FindNode and different faster SortLists
 *
 * Revision 1.4  95/10/26  16:17:42  idr
 * Changed FindName() to use a slightly more optimal loop condition.
 * Changed SortList() to use a radix sort rather than the yucky sort
 * that it was using before.  At this time, the old versions of both
 * functions still exist in the code.
 * 
 * Revision 1.3  95/10/20  15:03:47  idr
 * Fixed a bug in the Enqueue() function that caused nodes to be added to
 * the list in the wrong order.  I also cleaned that function and a couple
 * of others up a little bit so that they're a bit easier to read.
 * 
 * Revision 1.2  1995/09/05  02:38:01  idr
 * Move AddHead() and AddTail() so that I would not need to use any
 * bounus function prototypes to prevent compliation errors.
 *
 * Revision 1.1  1995/07/09  20:10:47  idr
 * Initial revision
 */

#include <stdlib.h>
#include <math.h>
#include <liblists.h>
#ifndef	NULL
#define	NULL	0
#endif

/******************************************************************************/
/* Name : NewList()
 *
 * Notes: The following passage is taken verbatim from _Amiga_ROM_Kernal
 *        _Reference_Manual:_Libraries_3rd_Edition_, page 490
 *
 * HEADER INITIALIZATION
 *
 * List headers must be properly initialized before use.  It is not
 * adequate to initialize the entire header to zero.  The head and tail
 * entries must have specific values.  The header must be initialized
 * as follows:
 *
 *  1. Set the lh_Head field to the address of lh_Tail.
 *  2. Clear the lh_Tail field.
 *  3. Set the lh_TailPred field to the address of lh_Head.
 *  4. Set lh_Type to the same data type as the nodes to be kept
 *     in the list. (Unless you are using a MinList).
 */
extern void nNewList(register struct nlist *list)
{
  list->head = (struct nnode *)&(list->tail);
  list->tail = NULL;
  list->tailpred = (struct nnode *)&(list->head);
  list->nodes = 0;
}

/******************************************************************************/
/* Name : AddHead()
 *
 * Notes: This function adds the specified node to the begining (head)
 *        of the specified list.
 */
void nAddHead(register struct nlist *list, register struct nnode *node)
{
  /* temporary pointer to a node  */
  register struct nnode *tempnode;

  /* Save the pointer to the old head node. */
  tempnode = list->head;
  /* Make ``node'' the new head. */
  list->head = node;
  /* Link ``node'' to its new neighbors. */
  node->succ = tempnode;
  node->pred = (struct nnode *)list;
  /* Link the old head to the new head. */
  tempnode->pred = node;
  list->nodes++;
}

/******************************************************************************/
/* Name : AddTail()
 *
 * Notes: This function adds the specified node to the end (tail) of
 *        the given list.
 */
extern void nAddTail(register struct nlist *list, register struct nnode *node)
{
  register struct nnode *tempnode;				/* temporary pointer to a node    */
  register struct nnode *tailnode;				/* pointer to the tail part of the list */

  /* Get a pointer to the tail part of the list header so that
   * we can just treat it like a normal node. 
   */
  tailnode = (struct nnode *)&(list->tail);
  /* Save a pointer to the old tail node. */
  tempnode = tailnode->pred;
  /* Set the new tail node pointer. */
  tailnode->pred = node;
  /* Link the new tail node to its new neighbors. */
  node->pred = tempnode;
  node->succ = tailnode;
  /* Line the old tail node to its new neighbors. */
  tempnode->succ = node;
  list->nodes++;
}

/******************************************************************************/
/* Name : RemHead()
 *
 * Notes: This function unlinks and returns the head node in the
 *        given list.  If the list is empty, this function will
 *        return NULL.
 */
struct nnode *nRemHead(register struct nlist *list)
{
  /* If the head node is the list node in the list, then
   * the list is really empty (see header comment for why
   * this is the case), so we need to return NULL. 
   */
  if (list->nodes) {
    register struct nnode *headnode;				/* pointer to the old head of the list  */
    register struct nnode *tempnode;				/* pointer to temporary node   */

    /* Get a pointer to the old head of the list. */
    headnode = list->head;
    /* Get the node after the old head. */
    tempnode = headnode->succ;
    /* Link the header and the node after the old head node
     * to each other. 
     */
    list->head = tempnode;
    tempnode->pred = (struct nnode *)list;
    /* Unlink the old head from the list. */
    headnode->succ = headnode->pred = NULL;
    /* Return the old head. */
    list->nodes--;

    return (headnode);
  }
  else
    return 0;
}

/******************************************************************************/
/* Name : RemTail()
 *
 * Notes: This function unlinks and returns the tail node in the
 *        given list.  If the list is empty, this function will
 *        return NULL.
 */
struct nnode *nRemTail(register struct nlist *list)
{
  if (list->nodes) {
    register struct nnode *tailnode;				/* pointer to the old tail of the list  */
    register struct nnode *tempnode;				/* pointer to temporary node   */

    tailnode = list->tailpred;
    tempnode = tailnode->pred;
    list->tailpred = tempnode;
    tempnode->succ = (struct nnode *)&(list->tail);
    list->nodes--;

    return (tailnode);
  }
  else
    return 0;
}

/******************************************************************************/
/* Name : Remove()
 *
 * Notes: The following passage is taken verbatim from _Amiga_ROM_Kernal
 *        _Reference_Manual:_Libraries_3rd_Edition_, page 492
 *
 * The Remove() function is used to remove a specified node from a list.
 * For example, Remove(node) will remove the specified node from whatever
 * list it is in.  To be removed, a node must actually be in a list.  If
 * you attempt to remove a node that is not in a list, you will cause
 * serious system problems.
 */
void nRemove(register struct nlist *list, register struct nnode *node)
{
  /* Line the node's neighbors to each other, rather than to node. */
  node->pred->succ = node->succ;
  node->succ->pred = node->pred;
  /* Unlink the node's neighbors from the node. */
  node->succ = node->pred = NULL;
  list->nodes--;
  return;
}

/******************************************************************************/
/* Name : Insert()
 *
 * Notes: The following passage is taken verbatim from _Amiga_ROM_Kernal
 *        _Reference_Manual:_Libraries_3rd_Edition_, page 492
 *
 * The Insert() function is used for inserting a new node into any
 * position in a list.  It always inserts the node following a
 * specified node that is already part of the list.  For example,
 * Insert(header, node, pred) inserts the node ``node'' after the node
 * ``pred'' in the specified list.  If the pred node points to the list
 * header or is NULL, the new node will be inserted at the head of the
 * list.  Similarly, if the pred node points to the lh_Tail of the
 * list, the new node will be inserted at the tail of the list.  However,
 * both of these actions can be better accomplished with the functions
 * mentioned in the "Special Case Insertion" section below.
 */
void nInsert(register struct nlist *list, register struct nnode *node, register struct nnode *pred)
{
  /* If the pointer to ``pred'' is NULL, then ``node'' will become
   * the head of the list. 
   */
  if (!pred) {
    nAddHead(list, node);
  }
  else {
    register struct nnode *tempnode;				/* temporary pointer to a node  */

    /* Save the pointer to the node that comes after ``pred''. */
    tempnode = pred->succ;
    /* Link ``pred'' to its new successor. */
    pred->succ = node;
    /* Link ``node'' to its new neighbors. */
    node->pred = pred;
    node->succ = tempnode;
    /* Link pred's old successor back to its new predicessor. */
    tempnode->pred = node;
    list->nodes++;
  }
}

/******************************************************************************/
/* Name : preInsert()
 *
 * Notes: The preInsert() function is used for inserting a new node into any
 *        position in a list.  It always inserts the node preceding a
 *        specified node that is already part of the list.  For example,
 *        Insert(header, node, succ) inserts the node ``node'' before the node
 *        ``succ'' in the specified list.  If the succ node points to the list
 *        header or is NULL, the new node will be inserted at the head of the
 *        list.  Similarly, if the succ node points to the lh_Tail of the
 *        list, the new node will be inserted at the tail of the list.
 *        However, both of these actions can be better accomplished with the
 *        functions mentioned in the "Special Case Insertion" section below.
 */
void npreInsert(register struct nlist *list, register struct nnode *node, register struct nnode *succ)
{
  /* If the pointer to ``succ'' is NULL, then ``node'' will become
   * the head of the list. 
   */
  if (!succ || !succ->pred) {
    nAddHead(list, node);
  }
  else {
    register struct nnode *tempnode;				/* temporary pointer to a node  */

    /* Save the pointer to the node that comes before ``succ''. */
    tempnode = succ->pred;
    /* Link ``pred'' to its new Predecessor */
    succ->pred = node;
    /* Link ``node'' to its new neighbors. */
    node->succ = succ;
    node->pred = tempnode;
    /* Link pred's old predecesor back to its new successor. */
    tempnode->succ = node;
    list->nodes++;
  }
}

/******************************************************************************/
/* Name : Enqueue()
 *
 * Notes: The following passage is taken verbatim from _Amiga_ROM_Kernal
 *        _Reference_Manual:_Libraries_3rd_Edition_, page 492
 *
 * PRIORITIZED INSERTION
 *
 * The list functions discussed so far do not make use of the priority
 * field in a Node.  The Enqueue() function is equivalent to Insert(),
 * except that it inserts nodes into a list sorting them according to
 * their priority.  It keeps the higher-priority nodes towards the head
 * of the list.  All nodes passed to this function must have their
 * priority and name assigned prior to the call.  Enqueue(header, mynode)
 * inserts ``mynode'' behind the lowest priority node with a priority
 * greater than or qual to ``mynode's''.  For Enqueue() to work properly,
 * the list ``mylist'' already be sorted according to priority.  Because the
 * highest priority node is at the head of the list, the RemHead() function
 * will remove the highest priority node.  Likewise, RemTail() will remove
 * the lowest priority node.
 *
 *      FIFO Is Used For The Same Priority.  If you add a node that has
 *      the same priority as another node in the queue, Enqueue() will
 *      use FIFO ordering.  The new node is inserted following the list
 *      node of equal priority.
 */
struct nnode *nFindNode(register struct nnode *startnode,
			register int numnodes, register int data)
{
  register int middle = numnodes - 1;
  register int shift;

  while ((shift = middle >> 1) > 0) {				/* real tries to find node is ln2(rgbnodes) */
    /* find the middle */
    register int loop = shift;
    register struct nnode *middnode = startnode;

    while ((loop--) > 0)					/* this eats most of the time */
      middnode = middnode->succ;

    if (data < middnode->data) {				/* if less than jump */
      startnode = middnode;
      middle = middle - shift;
    }
    else if (data > middnode->data)				/* if greater than stay */
      middle = shift;
    else
      return middnode;
  }

  return startnode->data < data ? startnode->pred : startnode;
}

struct nnode *nFindNodeReverse(register struct nnode *startnode,
			       register int numnodes, register int data)
{
  register int middle = numnodes - 1;
  register int shift;

  while ((shift = middle >> 1) > 0) {				/* real tries to find node is ln2(rgbnodes) */
    /* find the middle */
    register int loop = shift;
    register struct nnode *middnode = startnode;

    while ((loop--) > 0)					/* this eats most of the time */
      middnode = middnode->succ;

    if (data > middnode->data) {				/* if greater than jump */
      startnode = middnode;
      middle = middle - shift;
    }
    else if (data < middnode->data)				/* if less than stay */
      middle = shift;
    else
      return middnode;
  }

  return startnode->data < data ? startnode : startnode->pred;
}

void nEnqueue(register struct nlist *list, register struct nnode *node)
{
  register int data = node->data;

  /* special case for empty list element */
  if (!list->nodes)
    nAddHead(list, node);
  /* special case for first list element */
  else if (data >= list->head->data)
    nAddHead(list, node);
  /* special case for last list element */
  else if (data <= list->tailpred->data)
    nAddTail(list, node);
  /* we must walk the tree */
  else
    nInsert(list, node, nFindNode(list->head, list->nodes, data));
}

void nEnqueueReverse(register struct nlist *list, register struct nnode *node)
{
  register int data = node->data;

  /* special case for empty list element */
  if (!list->nodes)
    nAddHead(list, node);
  /* special case for first list element */
  else if (data <= list->head->data)
    nAddHead(list, node);
  /* special case for last list element */
  else if (data >= list->tailpred->data)
    nAddTail(list, node);
  /* we must walk the tree */
  else
    nInsert(list, node, nFindNodeReverse(list->head, list->nodes, data));
}

/******************************************************************************/
/* Name : GetHead()
 *
 * Notes: This function is very similar in concept to RemHead(), with the
 *        exception that it doesn't modify the linked list.
 */
struct nnode *nGetHead(register struct nlist *list)
{
  return ((list->head->succ == NULL) ? NULL : list->head);
}

/******************************************************************************/
/* Name : GetTail()
 *
 * Notes: This function is very similar in concept to RemTail(), with the
 *        exception that it doesn't modify the linked list.
 */
extern struct nnode *nGetTail(register struct nlist *list)
{
  return ((list->tailpred->pred == NULL) ? NULL : list->tailpred);
}

/******************************************************************************/
/* Name : SuccNode()
 *
 * Notes: This function returns a pointer to the next node, if one exists.
 *
 */
extern struct nnode *nSuccNode(register struct nnode *node)
{
  return ((node->succ->succ == NULL) ? NULL : node->succ);
}

/*
struct nnode *nCycleNode(register struct nnode *node)
{
  return ((node->succ->succ == NULL) ? *(((struct nnode *)node->succ) - 1) : node->succ);
}
 */

/******************************************************************************/
/* Name : MoveList()
 *
 * Notes: This function is used to take the nodes that are in one list and
 *        put them in another list.  It is important to note, that the
 *        source list is NOT VALID AFTER THIS CALL.  Also, any nodes that
 *        might already be in the destination list will be LOST.
 */
void nMoveList(register struct nlist *slist, register struct nlist *dlist)
{
  register struct nnode *node;

  /* First, make the destination list point to its new head and tail
   * nodes.  Also, init. the lh_Tail value.
   */
  dlist->head = slist->head;
  dlist->tail = NULL;
  dlist->tailpred = slist->tailpred;
  dlist->nodes = slist->nodes;
  /* Now, make the new tail node point to the list header. */
  node = dlist->tailpred;
  node->succ = (struct nnode *)&(dlist->tail);
  /* Now, make the new head node point back at the list header. */
  node = dlist->head;
  node->pred = (struct nnode *)&(dlist->head);
}

/******************************************************************************/
/* Name : MoveNode()
 *
 */
void nMoveNodeSorted(register struct nlist *slist, register struct nlist *dlist, register struct nnode *node)
{
  nRemove(slist, node);
  nEnqueue(dlist, node);
}

void nMoveNodeSortedReverse(register struct nlist *slist, register struct nlist *dlist, register struct nnode *node)
{
  nRemove(slist, node);
  nEnqueueReverse(dlist, node);
}

/******************************************************************************/
/* Name : AppendList()
 *
 * Notes: This function takes pointer to two list header, preList and
 *        postList, and creates a list that is a combination of the
 *        two, with the nodes in preList appearing first, followed by
 *        the nodes of postList.  The resultant list is linked with the
 *        header specified by preList.
 */
void nAppendList(register struct nlist *prelist, register struct nlist *postlist)
{
  /* If the list that we were going to append to is empty, then the
   * resultant list is just the list that we were going to append.
   * The easy way to do this is to just copy the postlist to the
   * prelist header.
   */
  if (!prelist->nodes) {
    nMoveList(postlist, prelist);
    nNewList(postlist);
  }
  /* If the list that we were going to append is empty, then we don't
   * really have a chore to do.  Let's return to the caller now.
   */
  else if (postlist->nodes) {
    register struct nnode *lastOfPre = prelist->tailpred;
    register struct nnode *firstOfPost = postlist->head;
    register struct nnode *lastOfPost;

    /* This is the point where this routine gets more than just a
     * little complex.  This would make a good assignment for 3rd term,
     * first year CS students.  It would weed the wimps out from the
     * big dogs. :)
     *
     * There are actually two steps to this process.  The first step is
     * to link the node at the end of the prelist to the node at the
     * start of the postlist.  The second step is to link the node at
     * the end of the postlist to the tail part of the list header.
     */
    lastOfPre->succ = firstOfPost;
    firstOfPost->pred = lastOfPre;
    lastOfPost = postlist->tailpred;
    lastOfPost->succ = (struct nnode *)&(prelist->tail);
    prelist->tailpred = lastOfPost;
    prelist->nodes += postlist->nodes;
    /* The last step is to make sure the user won't do something totally
     * retarded and use the postlist.  We'll do that by clearing the
     * list out.
     */
    nNewList(postlist);
  }
}

/******************************************************************************/
/* Name : PrependList()
 *
 * Notes: This function takes pointer to two list header, preList and
 *        postList, and creates a list that is a combination of the
 *        two, with the nodes in preList appearing first, followed by
 *        the nodes of postList.  The resultant list is linked with the
 *        header specified by postList.
 */
void nPrependList(register struct nlist *postList, register struct nlist *preList)
{
  /* This is kind of a cheesy way to do this, but it works. :) The
   * main reason that I did this routine this way is that it is a
   * LOT less code, and I don't think that PrependList() will, in
   * general, be used as much as AppendList().
   */
  nAppendList(preList, postList);
  nMoveList(preList, postList);
  nNewList(preList);
}

/******************************************************************************/
/* Name : SortList()
 *
 * Notes: This function is used to accomplish a slow and sleazy sorting of a
 *        list by copying the list into a temporary list and sorting each
 *        member as it gets copied.  Smaller Node Priorities get sorted to the
 *        bottom.
 */
#define RADIX		32
#define RADIX_DIVIDE	(1<<7)					/* special case for -128 upto 127 */
#define RADIX_OFFSET	(RADIX/2)				/* special case for -128 upto 127 */

/*
 * the list contains data as follows:
 *  es existieren mehr werte fuer niedrige zahlen und weniger werte fuer hohe zahlen:
 *   100 werte im bereich 0-100
 *   50 werte im bereich 100-200
 *   25 werte im bereich 200-300
 *  der groeszte wert ist nicht berechenbar und liegt in irgendeinem bereich
 */
void nSortListWeighted(register struct nlist *list, register int radix)
{
  register struct nlist **bucket;				/* Sorting buckets.    */
  register struct nnode *node = (struct nnode *)list;		/* Temporary list node pointer. */
  register int lcv;						/* Local counter variable.   */
  register int radix_divide = 0;				/* the greatest data in a node */

  /* allocate number of lists
   */
  if ((bucket = malloc(radix * sizeof(struct nlist *))) == 0)
    return;

  for (lcv = radix - 1; lcv >= 0; lcv--)
    if ((bucket[lcv] = malloc(sizeof(struct nlist))) == 0)
      return;

  /* get the highest data-value
   */
  for (lcv = list->nodes; lcv > 0; lcv--) {			/* nodes !!! */
    node = node->succ;
    if (node->data > radix_divide)
      radix_divide = node->data;
  }
  radix_divide = sqrt(radix_divide) / radix;			/* the worth */
  radix_divide++;

  /* First, initialize all of the buckets that we will be sorting the
   * nodes into.
   */
  for (lcv = radix - 1; lcv >= 0; lcv--)
    nNewList(bucket[lcv]);

  /* Now, move every node out of the source list into the bucked that
   * it belongs in.
   */
  node = list->head;						/* Temporary list node pointer. */
  for (lcv = list->nodes - 1; lcv >= 0; lcv--) {
    register struct nnode *succnode = node->succ;
    register int radix2 = sqrt(node->data) / radix_divide;

    nEnqueue(bucket[radix2], node);
    node = succnode;
  }

  /* Now merge all of the buckets together to form the final, sorted
   * linked list.
   */
  nMoveList(bucket[radix - 1], list);
  for (lcv = radix - 2; lcv >= 0; lcv--)
    nAppendList(list, bucket[lcv]);

  for (lcv = radix - 1; lcv >= 0; lcv--)
    free(bucket[lcv]);
  free(bucket);
}

void nSortListWeightedReverse(register struct nlist *list, register int radix)
{
  register struct nlist **bucket;				/* Sorting buckets.    */
  register struct nnode *node = (struct nnode *)list;		/* Temporary list node pointer. */
  register int lcv;						/* Local counter variable.   */
  register int radix_divide = 0;				/* the greatest data in a node */

  /* allocate number of lists
   */
  if ((bucket = malloc(radix * sizeof(struct nlist *))) == 0)
    return;

  for (lcv = radix - 1; lcv >= 0; lcv--)
    if ((bucket[lcv] = malloc(sizeof(struct nlist))) == 0)
      return;

  /* get the highest data-value
   */
  for (lcv = list->nodes; lcv > 0; lcv--) {			/* nodes !!! */
    node = node->succ;
    if (node->data > radix_divide)
      radix_divide = node->data;
  }
  radix_divide = sqrt(radix_divide) / radix;			/* the worth */
  radix_divide++;

  /* First, initialize all of the buckets that we will be sorting the
   * nodes into.
   */
  for (lcv = radix - 1; lcv >= 0; lcv--)
    nNewList(bucket[lcv]);

  /* Now, move every node out of the source list into the bucked that
   * it belongs in.
   */
  node = list->head;						/* Temporary list node pointer. */
  for (lcv = list->nodes - 1; lcv >= 0; lcv--) {
    register struct nnode *succnode = node->succ;
    register int radix2 = sqrt(node->data) / radix_divide;

    nEnqueueReverse(bucket[radix2], node);
    node = succnode;
  }

  /* Now merge all of the buckets together to form the final, sorted
   * linked list.
   */
  nMoveList(bucket[radix - 1], list);
  for (lcv = radix - 2; lcv >= 0; lcv--)
    nPrependList(list, bucket[lcv]);

  for (lcv = radix - 1; lcv >= 0; lcv--)
    free(bucket[lcv]);
  free(bucket);
}

/*
 * the list contains data as follows:
 *  die werte sind linear im daten-bereich verteilt
 *  der groeszte wert ist nicht berechenbar und liegt in irgendeinem bereich
 */
void nSortListLinear(register struct nlist *list, register int radix)
{
  register struct nlist **bucket;				/* Sorting buckets.    */
  register struct nnode *node = (struct nnode *)list;		/* Temporary list node pointer. */
  register int lcv;						/* Local counter variable.   */
  register int radix_divide = 0;				/* the greatest data in a node */

  /* allocate number of lists
   */
  if ((bucket = malloc(radix * sizeof(struct nlist *))) == 0)
    return;

  for (lcv = radix - 1; lcv >= 0; lcv--)
    if ((bucket[lcv] = malloc(sizeof(struct nlist))) == 0)
      return;

  /* get the highest data-value
   */
  for (lcv = list->nodes; lcv > 0; lcv--) {			/* nodes !!! */
    node = node->succ;
    if (node->data > radix_divide)
      radix_divide = node->data;
  }
  radix_divide = radix_divide / radix;				/* the worth */
  radix_divide++;

  /* First, initialize all of the buckets that we will be sorting the
   * nodes into.
   */
  for (lcv = radix - 1; lcv >= 0; lcv--)
    nNewList(bucket[lcv]);

  /* Now, move every node out of the source list into the bucked that
   * it belongs in.
   */
  node = list->head;						/* Temporary list node pointer. */
  for (lcv = list->nodes - 1; lcv >= 0; lcv--) {
    register struct nnode *succnode = node->succ;
    register int radix2 = node->data / radix_divide;

    nEnqueue(bucket[radix2], node);
    node = succnode;
  }

  /* Now merge all of the buckets together to form the final, sorted
   * linked list.
   */
  nMoveList(bucket[radix - 1], list);
  for (lcv = radix - 2; lcv >= 0; lcv--)
    nAppendList(list, bucket[lcv]);

  for (lcv = radix - 1; lcv >= 0; lcv--)
    free(bucket[lcv]);
  free(bucket);
}

void nSortListLinearReverse(register struct nlist *list, register int radix)
{
  register struct nlist **bucket;				/* Sorting buckets.    */
  register struct nnode *node = (struct nnode *)list;		/* Temporary list node pointer. */
  register int lcv;						/* Local counter variable.   */
  register int radix_divide = 0;				/* the greatest data in a node */

  /* allocate number of lists
   */
  if ((bucket = malloc(radix * sizeof(struct nlist *))) == 0)
    return;

  for (lcv = radix - 1; lcv >= 0; lcv--)
    if ((bucket[lcv] = malloc(sizeof(struct nlist))) == 0)
      return;

  /* get the highest data-value
   */
  for (lcv = list->nodes; lcv > 0; lcv--) {			/* nodes !!! */
    node = node->succ;
    if (node->data > radix_divide)
      radix_divide = node->data;
  }
  radix_divide = radix_divide / radix;				/* the worth */
  radix_divide++;

  /* First, initialize all of the buckets that we will be sorting the
   * nodes into.
   */
  for (lcv = radix - 1; lcv >= 0; lcv--)
    nNewList(bucket[lcv]);

  /* Now, move every node out of the source list into the bucked that
   * it belongs in.
   */
  node = list->head;						/* Temporary list node pointer. */
  for (lcv = list->nodes - 1; lcv >= 0; lcv--) {
    register struct nnode *succnode = node->succ;
    register int radix2 = node->data / radix_divide;

    nEnqueueReverse(bucket[radix2], node);
    node = succnode;
  }

  /* Now merge all of the buckets together to form the final, sorted
   * linked list.
   */
  nMoveList(bucket[radix - 1], list);
  for (lcv = radix - 2; lcv >= 0; lcv--)
    nPrependList(list, bucket[lcv]);

  for (lcv = radix - 1; lcv >= 0; lcv--)
    free(bucket[lcv]);
  free(bucket);
}

/*
 * the list contains data as follows:
 *  die werte sind linear im daten-bereich verteilt
 *  der groeszte wert ist berechenbar und wird angegeben
 */
void nSortListLinearMax(register struct nlist *list, register int radix, register int radix_divide)
{
  register struct nlist **bucket;				/* Sorting buckets.    */
  register struct nnode *node = (struct nnode *)list;		/* Temporary list node pointer. */
  register int lcv;						/* Local counter variable.   */

  /* allocate number of lists
   */
  if ((bucket = malloc(radix * sizeof(struct nlist *))) == 0)
    return;

  for (lcv = radix - 1; lcv >= 0; lcv--)
    if ((bucket[lcv] = malloc(sizeof(struct nlist))) == 0)
      return;

  /* get the highest data-value
   */
  radix_divide = radix_divide / radix;				/* the worth */
  radix_divide++;

  /* First, initialize all of the buckets that we will be sorting the
   * nodes into.
   */
  for (lcv = radix - 1; lcv >= 0; lcv--)
    nNewList(bucket[lcv]);

  /* Now, move every node out of the source list into the bucked that
   * it belongs in.
   */
  node = list->head;						/* Temporary list node pointer. */
  for (lcv = list->nodes - 1; lcv >= 0; lcv--) {
    register struct nnode *succnode = node->succ;
    register int radix2 = node->data / radix_divide;

    nEnqueue(bucket[radix2], node);
    node = succnode;
  }

  /* Now merge all of the buckets together to form the final, sorted
   * linked list.
   */
  nMoveList(bucket[radix - 1], list);
  for (lcv = radix - 2; lcv >= 0; lcv--)
    nAppendList(list, bucket[lcv]);

  for (lcv = radix - 1; lcv >= 0; lcv--)
    free(bucket[lcv]);
  free(bucket);
}

void nSortListLinearMaxReverse(register struct nlist *list, register int radix, register int radix_divide)
{
  register struct nlist **bucket;				/* Sorting buckets.    */
  register struct nnode *node = (struct nnode *)list;		/* Temporary list node pointer. */
  register int lcv;						/* Local counter variable.   */

  /* allocate number of lists
   */
  if ((bucket = malloc(radix * sizeof(struct nlist *))) == 0)
    return;

  for (lcv = radix - 1; lcv >= 0; lcv--)
    if ((bucket[lcv] = malloc(sizeof(struct nlist))) == 0)
      return;

  /* get the highest data-value
   */
  radix_divide = radix_divide / radix;				/* the worth */
  radix_divide++;

  /* First, initialize all of the buckets that we will be sorting the
   * nodes into.
   */
  for (lcv = radix - 1; lcv >= 0; lcv--)
    nNewList(bucket[lcv]);

  /* Now, move every node out of the source list into the bucked that
   * it belongs in.
   */
  node = list->head;						/* Temporary list node pointer. */
  for (lcv = list->nodes - 1; lcv >= 0; lcv--) {
    register struct nnode *succnode = node->succ;
    register int radix2 = node->data / radix_divide;

    nEnqueueReverse(bucket[radix2], node);
    node = succnode;
  }

  /* Now merge all of the buckets together to form the final, sorted
   * linked list.
   */
  nMoveList(bucket[radix - 1], list);
  for (lcv = radix - 2; lcv >= 0; lcv--)
    nPrependList(list, bucket[lcv]);

  for (lcv = radix - 1; lcv >= 0; lcv--)
    free(bucket[lcv]);
  free(bucket);
}

#endif								/* LISTS_C */
