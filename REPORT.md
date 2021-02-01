#  Submission Report Project 1:

For our queue, we implemented it using a linked list consisting of two structur
es. One was a node made up of a node pointer called next to keep track of the 
next item in our list and a void pointer to hold the datatypes we would be send
ing our queue. Our queue structure was essentially the linked list with a head 
and tail node and an int to keep track of the queue length. The implementation 
of the linked list that we used is fairly common and easy to implement with the
need for only a simple node structure to make it. It also made the queue easy 
to implement since it only required a head and tail node. Creating and destro
ying our queue was a matter of mallocing, intializing when we were creating and
and freeing the struct when we were destroying. To enqueue, we had to change the
tail nodes to point to the node that we were enqueueing and in the case that the
node we were pushing was the first node of the queue, both the head and tail wer
e changed. To dequeue, we had to change where the head was pointing and if it 
was the last item in the queue, the tail was changed as well. In both enqueueing
and dequeueing, the curr_length of the queue was either incremented or decrement
ed respectively. Delete has two temporary nodes, one of which kept track of the
current and previous node. The current node would search down the linked list
with the previous node following one behind. If the item was found, previous's
next could change to the temp node's next instead of temp to symbolize the del
etion of the temp node and it could the be freed. In case the first item was 
to be deleted, a case for that was placed at the beginning of the function to 
avoid missing it as temp starts on head's next node and not head. For iterate, 
a temp node was created and a while loop was run checking that temp was not 
null. Within the loop, the function sent was called using the the temp node's
item and arg. The return value of the funtion could be checked and the iterat
e fucntion could return if necessary. If not, temp would become temp next and
the process could be repeated. Many tests were run to ensure the queue's func
tionality was correct. Multiple enqueue/queue operation were performed, curr_
length was check to be correct, attempting to enqueue a null item, dequeue an
empty queue, delete was test to ensure the first item of a duplicate was dele
ted, attempting to delete something that wasn't there, and iterate was tested
using your test as well as using a callback print function was used to consta
ntly print the queue during each test ran.
For our uthread file, we started by having an enum to represent each of the 
four possible states of the threads to allow for easy coding. To represent 
the thread control block, we cretaed a struct that contained a tid, a pointer
to the context to allow for easy referencing, the thread's state, a pointer 
the top of the thread's stack, a retval variable to store the return value 
and check against when we have zombie threads, and a pair_tid for joining thr
eads. We used a few global variables to allow all the user library to have ac
cess to the pertinent information such as a thread_ID value to assign new, un
ique thread IDs to, queues to contain ready, blocked, and zombie threads, the
current active thread, a context to the main thread that is created once we a
ttempt to create a thread. Our program only really understand the idea of swa
pping context to change threads; however, that requires at least two threads,
so we created a main thread that can be used in a context switch when it bec
omes necessary and doesn't try to switch with something that isn't there.
Upon creating a thread, our function first checks if this is the first thread
being created. If it is, then all the queues for the program will be created 
and a dummy "main" thread is created, initialized, and our global active thre
ad is set to this thread. Initializing a thread consisted of callocing a new 
thread and mallocing its context pointer, then top_of_stack would call the pr
ovided alloc_stack function, thread state would be set to read, an id would 
be assigned, and thread_ID would be incremented. Notice that init context
is not actually run on this thread. After we set upt our program, another thr
ead is initialized, but this time a context is initialized too using the func
and arg sent toe thread_create. Then the thread is enqueued into ready. Now, 
we have an active thread, the dummy main, and a thread enqueued in ready to be 
run.
For our yield function we declared two TCB* thread items. The first of which is
curr_thread that we set to the active thread to allow use of the active thread 
elements before we enqueue the active thread into the ready queue if the active 
thread's status is not blocked. The second TCB* we use is the oldest_thread whi
ch we get from dequeueing from the ready queue. The next steps to update the 
active thread to the oldest thread since we are yielding and swap contexts.
For our thread join we used a single TCB* and serach through the zombie queue f
or the tid of the active running thread, if the zombie queue actually contains 
the thread, then we set the retval parameter to that threads return value, dele
te the thread from the zombie queue as well as its stack. If the thread ws not 
found in the zombie queue then the current thread must not be finished and stil
l needs to be ran at a later time, so we change it's status to blocked and push
it onto the blocked queue to be later retrieved.
Our exit function uses two TCB*, one that is equal to the top of the ready queu
e that we called next thread. Our second TCB* was used called reference thread 
was found by using the iterate function in conjunction with a callback function 
that we wrote. The functionality was to search through the blocked queue and fi
nd the first tid that matched that of the next thread. If one was found, then 
we could schedule that blocked thread, so we would change its status to ready a
nd push it onto the ready_queue and delete it from the blocked queue using the 
delete function. If no matching tid is found then, the we push the thread into 
zombie as it must be done. After updating the corresponding return value for th
e active, we check to ensure the ready queue has a thread to dequeue and then 
we dequeue that thread, store the context of the active thread, set the active 
thread to the next thread that we found, and then swap the context to change 
running threads
For our preempting function in order to enable and disable the preemption, we 
called sigpromask with either SIG_BLOCK or SIG_UNBLOCK if we are enabling or d
isabling respectively. 
