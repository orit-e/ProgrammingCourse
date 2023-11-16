typedef struct cqueue
{
	void **members;
	int front;
	int rear;
	int capacity;
}cqueue_t;

cqueue_t *CircularQCreate(size_t capacity):
1. Declare new_cqueue + initialize with malloc of size: sizeof(cqueu_t) + 
	sizeof(void *) * capacity.
2. Check if malloc was success. if not, return NULL;
3. if malloc was success, insert: new_cqueue->capacity = capacity; 
	new_cqueue->members = (void **)((char *)new_cqueue + sizeof(cqueu_t));
	new_cqueue->front = -1; and new_cqueue->rear = -1;
4. return new_cqueue;

void CircularQDestroy(cqueue_t *cqueue):
1. assert(NULL != cqueue);
2. memset(cqueue, 0, sizeof(cqueu_t) + sizeof(void *) * cqueue->capacity);
3. free(cqueue);
4. cqueue = NULL:

int CircularQIsEmpty(cqueue_t *cqueue):
1. assert(NULL != cqueue);
2. return (-1 == cqueue->front);

int CircularQIsFull(cqueue_t *cqueue):
1. assert(NULL != cqueue);
2. return (CircularQSize(cqueue_t *cqueue) == cqueue->capacity);

void *CircularQPeek(cqueue_t *cqueue):
1. assert(NULL != cqueue);
2. return cqueue->members[cqueue->front];

size_t CircularQSize(cqueue_t *cqueue):
1. assert(NULL != cqueue);
2. if CircularQIsEmpty(cqueue) than return 0;
3. else if cqueue->rear >= cqueue->front than return (cqueue->rear - 
	cqueue->front + 1);
4. else: return ((cqueue->capacity - cqueue->front) + cqueue->rear + 1);

int CircularQEnQueue(cqueue_t *cqueue, void *data):
1. assert(NULL != cqueue);
2. check CircularQIsFull(cqueue) if yes than: return 1;
3. check CircularQIsEmpty(cqueue) if yes than: cqueue->front = 0.
4. if cqueue->rear == cqueue->capacity - 1 or cqueue->rear == -1 than: 
	cqueue->rear = 0;
5. else: cqueue->rear += 1.
6. cqueue->members[cqueue->rear] = data;
7. return 0;

void CircularQDeQueue(cqueue_t *cqueue):
1. assert(NULL != cqueue);
2. assert(-1 != cqueue->front);
3. if cqueue->rear == cqueue->front than: both are now equal to -1.
4. else: cqueue->front = (cqueue->front + 1) % cqueue->capacity;


