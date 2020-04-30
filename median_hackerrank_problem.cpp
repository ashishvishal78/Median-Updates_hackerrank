/*input
7  
r 1  
a 1  
r 1 
r 1  
r 1  
r 2  
r 1
*/
#include <bits/stdc++.h>
#include <algorithm>
#include <fstream>
#include <sstream>
using namespace std;
#define int long long int

struct node
{
    int fre;
    int left_fre;
    int right_fre;
    int data;
    int ht;
    node* left;
    node* right;
};
node* root;
int height(node* nod)
{
    if(nod==0)
    return 0;
    else
    return nod->ht;
}
int get_fre(node* nod)
{
    if(nod==0)
    return 0;
    else
    return nod->fre+nod->left_fre+nod->right_fre;
}
int get_balance(node* nod)
{
    if(nod==0)
    return 0;
    else
    return height(nod->left)-height(nod->right);
}

node* create_node(int val)
{
    node* temp=new node();
    temp->data=val;
    temp->fre=1;
    temp->ht=1;
    temp->left=0;
    temp->right=0;
    temp->left_fre=0;
    temp->right_fre=0;
    return temp;
}

node* right_rotate(node* nod)
{
    node* x=nod->left;
    node* y=x->right;

    x->right=nod;
    nod->left=y;
    

    nod->ht=1+max(height(nod->left),height(nod->right));
    x->ht=1+max(height(x->left),height(x->left));

    nod->left_fre=get_fre(nod->left);
    nod->right_fre=get_fre(nod->right);

    //nod->fre=get_fre(nod->left)+get_fre(nod->right));
    //x->fre=get_fre(x->left)+get_fre(x->right);
    x->left_fre=get_fre(x->left);
    x->right_fre=get_fre(x->right);

    return x;
}


node* left_rotate(node* nod)
{
	//cout<<nod->data<<endl;
    node* x=nod->right;
    node* y=x->left;

    x->left=nod;
    nod->right=y;
    

    nod->ht=1+max(height(nod->left),height(nod->right));
    x->ht=1+max(height(x->left),height(x->right));

    nod->left_fre=get_fre(nod->left);
    nod->right_fre=get_fre(nod->right);

    //nod->fre=get_fre(nod->left)+get_fre(nod->right));
    //x->fre=get_fre(x->left)+get_fre(x->right);
    x->left_fre=get_fre(x->left);
    x->right_fre=get_fre(x->right);
    return x;
}

node* insert(node* root,int val)
{
    if(root==0)
    return create_node(val);
    else if(val==root->data)
    {
        root->fre+=1;
        return root;
    }
    else if(root->data>val)
    {
        root->left=insert(root->left,val);
    }
    else if(root->data<val)
    root->right=insert(root->right,val);

    root->ht=1+max(height(root->left),height(root->right));
    root->left_fre=get_fre(root->left);
    root->right_fre=get_fre(root->right);

    int bal=get_balance(root);

    if(bal>1&&root->left->data>val)
    return right_rotate(root);
    if(bal<-1&&root->right->data<val)
    return left_rotate(root);
    if(bal>1&&root->left->data<val)
    {
        root->left=left_rotate(root->left);
        return right_rotate(root);
    }
    if(bal<-1&&root->right->data>val)
    {
        root->right=right_rotate(root->right);
        return left_rotate(root);
    }
    return root;

}

node* find_min(node* root)
{
	while(root->left!=0)
		root=root->left;
	return root;
}
bool flag=0;
node* remove(node* root,int val)
{
    if(root==0)
    {
    	flag=1;
        //cout<<"Wrong!"<<endl;
        return root;
    }
    if(val<root->data)
    	root->left=remove(root->left,val);
    else if(val>root->data)
    	root->right=remove(root->right,val);
    else if(root->data==val)
    {
        if(root->fre>1)
        {
        	root->fre-=1;
        	return root;
        }
    	if( (root->left == 0) || (root->right == 0) )  
        {  
            node* temp = root->left ?  root->left : root->right;  
  
            // No child case  
            if (temp == 0)  
            {  
            	//cout<<"ALL NULL "<<val<<endl;
                temp = root;  
                root = 0; 
                //return root; 
            }  
            else // One child case  
            *root = *temp; // Copy the contents of  
                           // the non-empty child  
            free(temp);  
        }
        else
        {
        	node* temp=find_min(root->right);
        	root->data=temp->data;
        	root->right=remove(root->right,temp->data);
        }
    }
     if (root == NULL)  
    return root;

    root->ht=1+max(height(root->left),height(root->right));
    root->left_fre=get_fre(root->left);
    root->right_fre=get_fre(root->right);

    int bal=get_balance(root);

    if(bal>1 && get_balance(root->left)>=0)
    return right_rotate(root);
    if(bal<-1&&get_balance(root->right)<=0)
    return left_rotate(root);
    if(bal>1&&get_balance(root->left)<0)
    {
        root->left=left_rotate(root->left);
        return right_rotate(root);
    }
    if(bal<-1&&get_balance(root->right)>0)
    {
        root->right=right_rotate(root->right);
        return left_rotate(root);
    }
    return root;
}

int print_med(node* root,int total,int curr_ind,int req_ind)
{
    if(root==0)
    {
        //cout<<"Wrong!"<<endl;
        return 0;
    }
    else
    {
        if(req_ind<=(root->left_fre+curr_ind))
        {
            return print_med(root->left,total,curr_ind,req_ind);
        }
        else if(req_ind>(root->left_fre+curr_ind)&&req_ind<=(root->left_fre+root->fre+curr_ind))
        {
            //cout<<root->data<<endl;
            return root->data;
        }
        else //if(req_ind>(root->left_fre+curr_ind+root->fre))
        {
            curr_ind=root->left_fre+curr_ind+root->fre;
            return print_med(root->right,total,curr_ind,req_ind);
        }

    }
}

void print_inorder(node* root)
{
	if(root==0)
		return ;
	print_inorder(root->left);
	cout<<root->data<<" -> "<<root->fre<<"	";
	print_inorder(root->right);
}
bool search(node* root,int val)
{
	if(root==0)
		return false;
	else if(root->data>val)
		return search(root->left,val);
	else if(root->data<val)
		return search(root->right,val);
	else
		return true;
}
void median(vector<char> s,vector<int> X) 
{
	//file;
    for(int i=0;i<s.size();i++)
    {
    	//cout<<"i :"<<i<<" "<<s[i]<<" "<<X[i]<<endl;
        if(s[i]=='r')
        {
        	if(!search(root,X[i]))
            {
            	cout<<"Wrong!"<<endl;
            	continue;
            }
            root=remove(root,X[i]);
            //cout<<"sorted ele :";
            //print_inorder(root);
            //cout<<endl;
                        
            int total_fre=get_fre(root);
            if(total_fre==0)
            {
            	cout<<"Wrong!"<<endl;
            	continue;
            }
            long double ans=0;
            int req_ind=total_fre/2+1;
            ans+=print_med(root,total_fre,0,req_ind);

            if(total_fre%2==0)
            {
            	req_ind-=1;
            	ans+=print_med(root,total_fre,0,req_ind);
            	ans=ans/2;
            	//
            }
            if(floor(ans)==ceil(ans))
            {
            	int ans1=ans;
            	cout<<ans1<<endl;
            	continue;
            }
            //cout<<set_precision(1)<<endl;
            cout<<fixed;
            cout<<setprecision(1)<<ans<<endl;
            //cout<<ans<<endl;

        }
        else
        {
            root=insert(root,X[i]);
            //cout<<"sorted_element:";
            //print_inorder(root);
            //cout<<endl;
            int total_fre=get_fre(root);
            if(total_fre==0)
            {
            	cout<<"Wrong!"<<endl;
            	continue;
            }
            long double ans=0;
            int req_ind=total_fre/2+1;
            ans+=print_med(root,total_fre,0,req_ind);
            
            if(total_fre%2==0)
            {
            	req_ind-=1;
            	ans+=print_med(root,total_fre,0,req_ind);
            	ans=ans/2;
            	//
            }
            if(floor(ans)==ceil(ans))
            {
            	int ans1=ans;
            	cout<<ans1<<endl;
            	continue;
            }
            cout<<fixed;
            cout<<setprecision(1)<<ans<<endl;
        }
    }
    
    
}
int32_t main(void){

//Helpers for input and output
	//file;

	int N;
	cin >> N;
	
	vector<char> s;
    vector<int> X;
	char temp;
    int tempint;
	for(int i = 0; i < N; i++){
		cin >> temp >> tempint;
        s.push_back(temp);
        X.push_back(tempint);
	}
	
	median(s,X);
	return 0;
}