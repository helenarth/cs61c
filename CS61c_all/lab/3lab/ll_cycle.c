#include <stdio.h>
#define TRUE 1
#define FALSE 0

typedef struct node {
  int value;
  struct node *next;
} node;

int ll_has_cycle(node *head)
{
    node* tortoise = head;
    node* hare = head;

	if (hare==NULL)		//for the case of len zero list.
	{
		return FALSE;
	}
	//always has a next value but not necessarily a next next value
    while(TRUE)
	{
		if (hare->next != NULL)
		{	//if the next node is null than reached end of nodes
			//should be the value of hare previously.
			hare = hare->next;
			tortoise = tortoise->next;
		}
		
		if(hare->next == tortoise)
		{
			return TRUE;
			break;
		}
		else if (hare->next==NULL)
		{
			return FALSE;
			break;
		}
		else
		{
			hare = hare->next;
		}
    		
	}
}

void test_ll_has_cycle(void)
{
  int i;
  node nodes[25]; //enough to run our tests
  for(i=0; i < sizeof(nodes)/sizeof(node); i++) 
  {
    nodes[i].next = 0;
    nodes[i].value = 0;
  }
  nodes[0].next = &nodes[1];
  nodes[1].next = &nodes[2];
  nodes[2].next = &nodes[3];
  printf("Checking 1ST list for cycles. should be none, ll_has_cycle has %s cycle\n\n",
    ll_has_cycle(&nodes[0])?"a":"no");
  
  nodes[4].next = &nodes[5];
  nodes[5].next = &nodes[6];
  nodes[6].next = &nodes[7];
  nodes[7].next = &nodes[8];
  nodes[8].next = &nodes[9];
  nodes[9].next = &nodes[10];
  nodes[10].next = &nodes[4];
  printf("Checking 2ND list for cycles. should be a cycle, ll_has_cycle has %s cycle\n\n", ll_has_cycle(&nodes[4])?"a":"no");
  
  nodes[11].next = &nodes[12];
  nodes[12].next = &nodes[13];
  nodes[13].next = &nodes[14];
  nodes[14].next = &nodes[15];
  nodes[15].next = &nodes[16];
  nodes[16].next = &nodes[17];
  nodes[17].next = &nodes[14];
  printf("Checking 3RD list for cycles. should be a cycle, ll_has_cycle has %s cycle\n\n",
    ll_has_cycle(&nodes[11])?"a":"no");
  
  nodes[18].next = &nodes[18];
  printf("Checking 4TH list for cycles. should be a cycle, ll_has_cycle has %s cycle\n\n",
    ll_has_cycle(&nodes[18])?"a":"no");
  
  nodes[19].next = &nodes[20];
  nodes[20].next = &nodes[21];
  nodes[21].next = &nodes[22];
  nodes[22].next = &nodes[23];
  printf("Checking 5TH list for cycles. should be none, ll_has_cycle has %s cycle\n\n",
    ll_has_cycle(&nodes[19])?"a":"no");
  
  printf("Checking len-zero list for cycles. should be none, ll_has_cycle has %s cycle\n\n",
    ll_has_cycle(NULL)?"a":"no");
}

int
main(void)
{
  test_ll_has_cycle();
  getchar();
  return 0;
}
