#include "qlists.h"

static qList_Node_t* qList_NodeInit( void * const node );
static void qList_InsertAtFront( qList_t * const list, qList_Node_t * const node );
static void qList_InserAtBack( qList_t * const list, qList_Node_t * const node );
static qList_Node_t* qList_RemoveFront( qList_t * const list );
static qList_Node_t* qList_RemoveBack( qList_t * const list );
static qList_Node_t* qList_GetiNode( const qList_t *const list, const qList_Position_t position );
static qBool_t qList_ChangeContainer( qList_ForEachHandle_t h );
static void qList_GivenNodes_SwapBoundaries( qList_Node_t *n1, qList_Node_t *n2 );
static void qList_GivenNodes_SwapAdjacent( qList_Node_t *n1, qList_Node_t *n2 );
static void qList_GivenNodes_UpdateOuterLinks( qList_Node_t *n1, qList_Node_t *n2 );


#ifdef QLIST_D_HANDLING  /*use only if strictly necessary*/
static qList_MemAllocator_t qListMalloc = NULL; 
static qList_MemFree_t qListFree = NULL; 
#endif

/*============================================================================*/
/*void qList_Initialize( qList_t * const list )
 
Must be called before a list is used!  This initialises all the members of the 
list structure.

Parameters:

    - list : Pointer to the list being initialised.   

*/
void qList_Initialize( qList_t * const list ){
    if( NULL != list ){
        list->head = NULL;
        list->tail = NULL;
        list->size = 0u;
    }
}
/*=========================================================*/
static qList_Node_t* qList_NodeInit( void * const node ){
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    qList_Node_t *xNode = (qList_Node_t*)node; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    xNode->prev = NULL;
    xNode->next = NULL;
    xNode->container = NULL;
    return xNode;
}
/*=========================================================*/
static void qList_InsertAtFront( qList_t * const list, qList_Node_t * const node ){
    node->next = list->head;
    list->head->prev = node;
    list->head = node;
}
/*=========================================================*/
static void qList_InserAtBack( qList_t * const list, qList_Node_t * const node ){
    list->tail->next = node;
    node->prev = list->tail;
    list->tail = node;
}
/*=========================================================*/
static qList_Node_t* qList_RemoveFront( qList_t * const list ){
    qList_Node_t *removed;
    
    removed = list->head;
    list->head = removed->next;
    if( NULL == list->head ){
        list->tail = list->head;
    }
    else{
        list->head->prev = NULL;    
    }
    return removed;
}
/*=========================================================*/
static qList_Node_t* qList_RemoveBack( qList_t * const list ){
    qList_Node_t *removed;
    
    removed = list->tail;
    list->tail = removed->prev;
    if( NULL == list->tail ){
        list->head = list->tail;
    }
    else{
        list->tail->next = NULL;    
    }
    return removed;
}
/*=========================================================*/
static qList_Node_t* qList_GetiNode( const qList_t *const list, const qList_Position_t position ){
    qList_Node_t *iNode;
    qBase_t iPos = 0;

    for( iNode = list->head ; ( iPos < (qBase_t)position ) && ( NULL != iNode->next ) ; iNode = iNode->next ){
        ++iPos;
    }
    return iNode;
}
/*=========================================================*/
/*qBool_t qList_Insert( qList_t * const list, void * const node, const qList_Position_t position )
 
Insert an item into the list.

Parameters:

    - list : Pointer to the list.
    - node : A pointer to the node to be inserted
    - position : The position where the node will be inserted. Could be 
                 qList_AtFront, qList_AtBack or any other index number 
                 where the node will be inserted after.

Return value:

    qTrue if the item was successfully added to the list, othewise returns qFalse   

*/
qBool_t qList_Insert( qList_t *const list, void * const node, const qList_Position_t position ){
    qBool_t RetValue = qFalse;
 
    if( ( NULL != list ) && ( NULL != node ) && ( position >= (qList_Position_t)(-1) ) ){    
        if( qFalse == qList_IsMember( list, node )){
            qList_Node_t *newnode;

            newnode = qList_NodeInit( node );
            RetValue = qTrue;
            if( NULL == list->head ){ /*list is empty*/
                list->head = newnode; 
                list->tail = newnode;
            }
            /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
            else if( qList_AtFront == position ){
                qList_InsertAtFront( list, node ); /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
            }
            else if( position >= ( (qList_Position_t)list->size - 1 ) ){
                qList_InserAtBack( list, node ); /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
            }
            /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
            else{ /*insert the new node after the position*/
                qList_Node_t *iNode;

                iNode = qList_GetiNode( list, position );
                newnode->next = iNode->next;  /*  NEW -> (i+1)NODE */
                newnode->prev = iNode;        /*  iNODE <- NEW */
                iNode->next->prev = newnode;  /*  NEW <- (i+1)NODE  */
                iNode->next = newnode;        /*  iNODE -> NEW */  
            }                                 /*  result: iNODE <-> NEW <-> (i+1)NODE    */
            ++list->size;
            newnode->container = list;
        }
    }
    return RetValue;  
}
/*=========================================================*/           
/*void* qList_RemoveItself( void * const node )
 
If the node is member of a list, the node will be removed from it.

Parameters:

    - node : A pointer to the node

Return value:

    qTrue on Success. qFalse if removal can't be performed.  

*/ 
/*=========================================================*/           
qBool_t qList_RemoveItself( void * const node ){
    qBool_t RetValue = qFalse;
    
    if( NULL != node ){
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
        qList_Node_t *toRemove = (qList_Node_t*)node; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
        
        if( NULL != toRemove->container ){
            qList_t *list;

            list = (qList_t*)toRemove->container; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
            /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
            if( toRemove == list->head ){
                (void)qList_RemoveFront( list );          
            }
            else if( toRemove == list->tail ){
                (void)qList_RemoveBack( list );
            }
            else{
                toRemove->prev->next = toRemove->next;
                if( NULL != toRemove->next ){
                    toRemove->next->prev = toRemove->prev;
                }
            }
            --list->size;
            toRemove->container = NULL;
            RetValue = qTrue;
        }
    }
    return RetValue;
}
/*=========================================================*/           
/*void* qList_Remove( qList_t * const list, void * const node, const qList_Position_t position )
 
Remove an item from the list.

Parameters:

    - list : Pointer to the list.
    - node : A pointer to the node to be deleted (to ignore pass NULL )
             If the node is member or the list, use qList_RemoveItself
             to avoid overhead
    - position : The position of the node that will be removed. Could be 
                 qList_AtFront, qList_AtBack or any other index number.

Return value:

    A pointer to the removed node. NULL if removal can't be performed.  

*/ 
void* qList_Remove( qList_t * const list, void * const node, const qList_Position_t position ){
    qList_Node_t *removed = NULL;

    if( ( NULL != list->head ) && ( position >= (qList_Position_t)(-1) ) ){
        if ( qList_IsMember( list, node ) ){
            if( qList_RemoveItself( node ) ){
                /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
                removed = node; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
                /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
            }
        }
        else{
            if( position <= (qList_Position_t)0 ){
                removed = qList_RemoveFront( list );     
            }
            else if( position > ( (qList_Position_t)list->size - 1 ) ){
                removed = qList_RemoveBack( list );  
            }
            else{
                qList_Node_t *iNode;
                qBase_t LastIndex = ( (qBase_t)position - 1 );

                iNode = qList_GetiNode( list, (qList_Position_t)LastIndex );
                removed = iNode->next;       /*  <-> (inode0) <-> inode1 <-> inode2 */
                iNode->next = removed->next;
                if( NULL != removed->next ){
                    iNode->next->prev = iNode;
                }
            }
            removed->container = NULL;
            --list->size;
        }
    }
    return removed;
}
/*=========================================================*/           
/*qBool_t qList_Move( qList_t *const destination, qList_t *const source, const qList_Position_t position )

Moves(or merge) the entire list pointed by <source> to the list pointed by 
<destination> at location specified by <position>. 
After the move operation, this function leaves empty the list pointed 
by <source>.

Parameters:

    - destination : Pointer to the list where the <source> nodes are to be moved.
    - source : Pointer to the source list to be moved.
    - position : The position where <source> list will be inserted. Could be 
                 qList_AtFront, qList_AtBack or any other index number 
                 where the list will be inserted after.

Return value:

    qTrue if the move operation is performed successfully, otherwise 
    returns qFalse   

*/
qBool_t qList_Move( qList_t *const destination, qList_t *const source, const qList_Position_t position ){
    qBool_t RetValue = qFalse;

    if( ( NULL != destination ) && ( NULL != source ) && ( position >= (qList_Position_t)(-1) )  ) {    
        if( NULL != source->head){ /*source has items*/
            RetValue = qTrue;
            (void)qList_ForEach( source, qList_ChangeContainer, destination, QLIST_FORWARD, NULL );          
            if( NULL == destination->head ){ /*destination is empty*/
                destination->head = source->head;
                destination->tail = source->tail;
            }
            else if( qList_AtFront == position ){
                source->tail->next = destination->head;
                destination->head->prev = source->tail;
                destination->head = source->head;
            }
            else if( position >= ( (qList_Position_t)destination->size - 1 ) ){
                destination->tail->next = source->head;
                source->head->prev = destination->tail;
                destination->tail = source->tail;
            }
            else{ /*insert the new list after the position*/
                qList_Node_t *iNode;

                iNode = qList_GetiNode( destination, position );
                source->tail->next = iNode->next; 
                source->head->prev = iNode;
                iNode->next = source->head;
            }
            destination->size += source->size;
            qList_Initialize( source ); /*clean up source*/
        }
    }
    return RetValue;
}
/*=========================================================*/
static qBool_t qList_ChangeContainer( qList_ForEachHandle_t h ){
    qList_Node_t *xNode;

    if( qList_WalkThrough == h->stage ){
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/ 
        xNode = (qList_Node_t*)h->node; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
        /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
        xNode->container = h->arg; /*new container stored into <arg>*/
    }
    return qFalse;
}
/*=========================================================*/
/*qBool_t qList_IsMember( qList_t * const list,  void * const node )
 
Check if the node is member of the list.

Parameters:

    - list : Pointer to the list.
    - node : A pointer to the node

Return value:

    qTrue if the node belongs to the list, qFalse if it is not.  

*/ 
qBool_t qList_IsMember( qList_t * const list, void * const node ){
    qBool_t RetValue = qFalse;

    if( NULL != node ){
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
        qList_Node_t *xNode = node; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
        
        if( list  == (qList_t*)xNode->container ){ /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
            RetValue = qTrue;
        }
        /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    }
    return RetValue;
}
/*=========================================================*/
/*void* qList_GetFront( const qList_t * const list )
 
Get a pointer to the front item of the list

Parameters:

    - list : Pointer to the list.

Return value:

    A pointer to the front node. NULL if the list is empty  

*/ 
void* qList_GetFront( const qList_t *const list ){
    void *RetValue = NULL;

    if( NULL != list ){
        RetValue = (void*)list->head;
    }
    return RetValue;
}
/*=========================================================*/
/*void* qList_GetBack( const qList_t * const list )
 
Get a pointer to the back item of the list

Parameters:

    - list : Pointer to the list.

Return value:

    A pointer to the back node. NULL if the list is empty  

*/ 
void* qList_GetBack( const qList_t *const list ){
    void *RetValue = NULL;

    if( NULL != list ){
        RetValue = (void*)list->tail;
    }
    return RetValue;
}
/*=========================================================*/
/*qBool_t qList_IsEmpty( const qList_t * const list )
 
Check if the list is empty.

Parameters:

    - list : Pointer to the list.

Return value:

    qTrue if the list is empty, qFalse if it is not.  

*/ 
qBool_t qList_IsEmpty( const qList_t * const list ){
    qBool_t RetValue = qTrue;
    
    if( NULL != list ){
        RetValue = ( NULL == list->head )? qTrue : qFalse;
    }
    return RetValue;
}
/*=========================================================*/
/*void* qList_Length( const qList_t * const list )
 
Get the number of items inside the list

Parameters:

    - list : Pointer to the list.

Return value:

    The number of items of the list. 

*/ 
size_t qList_Length( const qList_t * const list ){
    size_t RetValue = 0u;

    if( NULL != list ){
        RetValue = list->size;
    }
    return RetValue;
}
/*=========================================================*/
/*qBool_t qList_Sort( qList_t * const list, qList_CompareFcn_t CompareFcn ) 

Sort the double linked list using the <CompareFcn> function to 
determine the order.
The sorting algorithm used by this function compares pairs of 
adjacent nodes by calling the specified <CompareFcn> function 
with pointers to them as arguments. The sort is performed only 
modifying node's links without data swapping, improving performance 
if nodes have a large storage.

Note: The function modifies the content of the list by reordering its 
elements as defined by <CompareFcn>.

Parameters:

    - list : Pointer to the list.
    - CompareFcn :  Pointer to a function that compares two nodes.
                    This function is called repeatedly by qList_Sort
                    to compare two nodes. It shall follow the 
                    following prototype:
                    qBool_t (*CompareFcn)(void *node1, void *node2)
                    Taking two pointers as arguments (both converted
                    to const void*). The function defines the order
                    of the elements by returning a Boolean data, where
                    a <qTrue> value indicates that element pointed by 
                    <node1> goes after the element pointed to by <node2>

Return value:

    qTrue if at least one reordering is performed over the list. 
*/
qBool_t qList_Sort( qList_t * const list, qList_CompareFcn_t CompareFcn ){
    qBool_t RetValue = qFalse;

    if( ( NULL != list ) && ( NULL != CompareFcn ) ){
        size_t count = list->size;

        if( count >= (size_t)2 ){ /*It is only worth running the algorithm if the list has two or more nodes*/
            qList_Node_t *current = NULL, *before, *after;
            _qList_CompareHandle_t xHandle;
            size_t i, j, n;
            qBool_t xRetCmp;

            for( i = (size_t)1; i < count; ++i ){
                current = list->head;
                n = count - i - (size_t)1;
                for( j = (size_t)0; j <= n; ++j ){ 
                    xHandle.n1 = current;
                    xHandle.n2 = current->next;
                    xRetCmp = CompareFcn( &xHandle );
                    if( qTrue == xRetCmp ) { /*compare adjacent nodes*/
                        before = current->prev;
                        after = current->next;

                        if( NULL != before){
                            before->next = after;
                        } 
                        else{
                            list->head = after; /* In case <before> pointer is null, <after> pointer should be the new head*/
                        }
                        current->next = after->next;
                        current->prev = after;

                        if( NULL != after->next ){
                            after->next->prev = current; /* <prev> pointer of after->next should be set to current. */
                        }

                        after->next = current;
                        after->prev = before;
                        RetValue = qTrue;
                    } 
                    else{
                        current = current->next; /* Go to next node only if current->data > current->next->data condition is false. */
                    }
                }
            }
            while( NULL != current->next ){ /*loop remaining nodes until find the new tail*/
                current = current->next;
            }
            list->tail = current;            
        }
    }
    return RetValue;
}
/*=========================================================*/
/*qBool_t qList_IteratorSet( qList_Iterator_t *iterator, qList_t *const list, void *NodeOffset, qList_Direction_t dir ){

Setup an instance of the given iterator to traverse the list.

Parameters:

    - iterator : Pointer to the iterator instance
    - list : Pointer to the list.
    - NodeOffset :  The start offset-node. To ignore, pass NULL-
    - dir : Use one of the following options:
               QLIST_FORWARD  : to go in forward direction.
               QLIST_BACKWARD :  to go in backward direction.


Return value:

    qTrue on success. Otherwise returns qFalse. 
*/
qBool_t qList_IteratorSet( qList_Iterator_t *iterator, qList_t *const list, void *NodeOffset, qList_Direction_t dir ){
    qBool_t RetValue = qFalse;

    if( ( NULL != list ) && ( NULL != iterator ) && ( ( &QLIST_FORWARD == dir ) || ( &QLIST_BACKWARD == dir) ) ){
        qList_Node_t *Offset;
        
        iterator->direction = dir;
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
        Offset = (qList_Node_t*)NodeOffset; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
        /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
        if( NULL != Offset){
            if( list == Offset->container ){
                iterator->next = Offset;
                RetValue = qTrue;
            }
        }
        else{
            iterator->next = ( QLIST_FORWARD == dir )? list->head : list->tail;
            RetValue = qTrue;
        }
    }
    return RetValue;
}
/*=========================================================*/
/*void* qList_IteratorGetNext( qList_Iterator_t *iterator )

Get the current node available in the iterator. After invoked, 
iterator will be updated to the next node.

Parameters:

    - iterator : Pointer to the iterator instance

Return value:

    Return the next node or NULL when no more nodes remain in the list.

*/
void* qList_IteratorGetNext( qList_Iterator_t *iterator ){
    void *iNode = NULL;

    if( NULL != iterator ){
        iNode = iterator->next;
        if( NULL != iNode ){
            iterator->next = iterator->direction( iterator->next );
        }
    }
    return iNode;
}
/*=========================================================*/
/*qBool_t qList_ForEach( qList_t *const list, const qList_NodeFcn_t Fcn, void *arg, qList_Direction_t dir, void *NodeOffset )
 
Operate on each element of the list.

Parameters:

    - list : Pointer to the list.
    - Fcn : The function to perform over the node. 
            Should have this prototype:
            qBool_t Function( void* Node, void *arg, qList_WalkStage_t stage )
            
            If <Function> returns qTrue, the walk-through loop
            will be terminated.

    - arg : Argument passed to <Fcn>
    - dir : Use one of the following options:
               QLIST_FORWARD  : to walk through the list forwards.
               QLIST_BACKWARD : to walk through the list backwards.
    - NodeOffset : If available, the list walk through will start from this node.  
                   To ignore, pass NULL.       

Return value:

    qTrue if the walk through was early terminated, otherwise returns qFalse.

*/ 
qBool_t qList_ForEach( qList_t *const list, const qList_NodeFcn_t Fcn, void *arg, qList_Direction_t dir, void *NodeOffset ){
    qBool_t RetValue = qFalse;
    
    if( ( NULL != list ) && ( NULL != Fcn ) && ( ( &QLIST_FORWARD == dir ) || ( &QLIST_BACKWARD == dir) ) ){
        if ( NULL != list->head ){  /*walk the list only if it has items*/
            qList_Node_t *iNode;
            qList_Node_t *adjacent; /*to allow i-node links to be changed in the walk throught*/
            _qList_ForEachHandle_t xHandle = { NULL, arg,  qList_WalkInit };

            adjacent = ( &QLIST_FORWARD == dir )? list->head : list->tail; /*select starting point*/
            if( NULL != NodeOffset ){ /*offset request?*/
                /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
                iNode = (qList_Node_t*)NodeOffset; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
                /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
                if( iNode->container == list ){ /*the offset belongs to the list?*/
                    adjacent = iNode; /*take offset as a starting point*/
                }
            }
            RetValue = Fcn( &xHandle ); /*run initial stage before looping through list*/
            if( qFalse == RetValue ){ /*check if the initial stage allows us to continue*/
                xHandle.stage = qList_WalkThrough;
                for( iNode = adjacent; NULL != iNode; iNode = adjacent ){ /*loop the list*/
                    adjacent = dir( iNode ); /*Save the adjacent node if the current node changes its links. */
                    xHandle.node = iNode;
                    RetValue = Fcn( &xHandle ); /*perform action over the node*/
                    if( RetValue ){ /*check if the last node handling breaks the loop*/
                        break;
                    }               
                }
                if( qFalse == RetValue ){ /*if the last node allows to continue, run the ending stage*/
                    xHandle.node = NULL;
                    xHandle.stage = qList_WalkEnd;
                    RetValue = Fcn( &xHandle ); 
                }
            }    
        }    
    }
    return RetValue;
}
/*=========================================================*/
qList_Node_t* _qNode_Forward( const qList_Node_t *const node ){
    return node->next;
}
/*=========================================================*/
qList_Node_t* _qNode_Backward( const qList_Node_t *const node){
    return node->prev;
}
/*=========================================================*/
/*qBool_t qList_Swap( void *node1, void *node2 )
 
Swap two nodes that belongs to the same list by changing its
own links.

Note: The list containing nodes will be updated if any node 
is part of the boundaries.


Parameters:

    - node1 : Pointer to the first node.
    - node2 : Pointer to the second node.

Return value:

    qTrue if the swap operation is performed. Otherwise returns qFalse.

*/ 
qBool_t qList_Swap( void *node1, void *node2 ){
    qBool_t RetValue = qFalse;

    if( ( NULL != node1 ) && ( NULL != node2) && ( node1 != node2 ) ){ 
        /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
        qList_Node_t *n1 = (qList_Node_t*)node1; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
        qList_Node_t *n2 = (qList_Node_t*)node2; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
        
        if( ( NULL != n1->container ) && ( n1->container == n2->container ) ){ /*nodes are part of the same list*/
            qList_Node_t *tmp1, *tmp2;

            if( n2->next == n1 ){
                n1 = (qList_Node_t*)node2; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */ 
                n2 = (qList_Node_t*)node1; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */              
            }          
            /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
            tmp1 = n1->prev; /*save links prior swap*/
            tmp2 = n2->next;           
            qList_GivenNodes_SwapBoundaries( n1, n2 );
            qList_GivenNodes_SwapAdjacent( n1, n2 );
            n2->prev = tmp1; /*restore previously saved links*/
            n1->next = tmp2;
            qList_GivenNodes_UpdateOuterLinks( n1, n2 );   
            RetValue = qTrue;
        }
    }
    return RetValue;
}
/*=========================================================*/
static void qList_GivenNodes_SwapBoundaries( qList_Node_t *n1, qList_Node_t *n2 ){
    qList_t *list;
    /*cstat -MISRAC2012-Rule-11.5 -CERT-EXP36-C_b*/
    list = n1->container; /* MISRAC2012-Rule-11.5,CERT-EXP36-C_b deviation allowed */
    /*cstat +MISRAC2012-Rule-11.5 +CERT-EXP36-C_b*/
    /*update the list links*/
    if( list->head == n1 ){
        list->head = n2;
    }
    else if( list->head == n2 ){
        list->head = n1;
    }    
    else{
        /*nothing to do here*/         
    }        
    if( list->tail == n1 ){
        list->tail = n2;
    }
    else if( list->tail == n2 ){
        list->tail = n1;
    }
    else{
        /*nothing to do here*/
    } 
}
/*=========================================================*/
static void qList_GivenNodes_SwapAdjacent( qList_Node_t *n1, qList_Node_t *n2 ){
    if( ( (n1->next == n2 ) && ( n2->prev == n1 ) ) || ( ( n1->prev == n2 ) && ( n2->next == n1 ) ) ){ /*adjacent nodes?*/
        n1->prev = n1->next; 
        n2->next = n2->prev;
    }
    else{
        n1->prev = n2->prev;
        n2->next = n1->next;
    }
}
/*=========================================================*/
static void qList_GivenNodes_UpdateOuterLinks( qList_Node_t *n1, qList_Node_t *n2 ){
    if( NULL != n1->prev ){
        n1->prev->next = n1;
    }
    if( NULL != n1->next ){
        n1->next->prev = n1;
    }
    if( NULL != n2->prev ){
        n2->prev->next = n2;
    }
    if( NULL != n2->next ){
        n2->next->prev = n2;
    } 
}

#ifdef QLIST_D_HANDLING  /*use only if strictly necessary*/
/*=========================================================*/
void qList_SetMemoryAllocation( qList_MemAllocator_t mallocFcn, qList_MemFree_t freeFcn  ){
    qListFree = freeFcn;
    qListMalloc = mallocFcn;
}
/*=========================================================*/
qBool_t qList_DInsert( qList_t *const list, void *Data, size_t Size, qList_Position_t position ){
    qBool_t RetValue = qFalse;
    
    if( ( NULL != qListMalloc ) && ( NULL != qListFree ) && ( Size > (size_t)0) ){
        void *NewNode = qListMalloc( Size );
        if( NULL != NewNode ){
            (void)memcpy( NewNode, Data, Size );
            RetValue = qList_Insert( list, NewNode, position );
            if( qFalse == RetValue ){
                qListFree( NewNode );
            }
        }
    }
    return RetValue;
}
/*=========================================================*/
void* qList_DRemove( qList_t * const list, void * const node, const qList_Position_t position ){
    void *removed = NULL;

    if( ( NULL != qListMalloc ) && ( NULL != qListFree ) ){
        removed = qList_Remove( list, node, position );
        if( NULL != removed ){
            qListFree( removed );        
        }    
    }
    return removed;
}
/*=========================================================*/
qBool_t qList_DRemoveItself( void * const node ){
    qBool_t RetValue = qFalse;
    
    if( ( NULL != qListMalloc ) && ( NULL != qListFree ) ){
        if( qList_RemoveItself( node ) ){
            qListFree( node );
            RetValue = qTrue;
        }
    }
    return RetValue;
}
/*=========================================================*/
#endif