#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 500

typedef struct student
{
    int student_id;
    char name[MAX];
    char group_partner[MAX];
    int group_id;
    int assignment_id;
    char deadline[MAX];
    char status[MAX];
    float offline_evaluation_marks;
    float viva_marks;
    struct student *next;
} student;

typedef struct assignment
{
    int assignment_id;
    char topic_name[MAX];
    char status[MAX];
    struct student_group *student_group_list;
    struct assignment *next;
} assignment;

typedef struct student_group
{
    int group_id;
    char status[MAX];
    float marks;
    struct student *student1;
    struct student *student2;
    struct student_group *next;
} student_group;

void password_login()
{
    char password[MAX];
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(password, "password123") == 0)
    {
        printf("Login successful!\n");
    }
    else
    {
        printf("Incorrect password. Please try again.\n");
        password_login();
    }
}

void insert_student_record(student **student_list_ptr, assignment **assignment_list_ptr, int student_id, char *name, char *group_partner, int group_id, int assignment_id, char *deadline, char *status, float offline_evaluation_marks, float viva_marks, char *topic_name)
{
	
    student *last_student = *student_list_ptr;
    while (last_student != NULL)
    {
        if (last_student->student_id == student_id)
        {
            printf("Error: Student with ID %d already exists\n", student_id);
            return; 
        }
        last_student = last_student->next;
    }
    student *new_student = (student*)malloc(sizeof(student));
    new_student->student_id = student_id;
    strcpy(new_student->name, name);
    strcpy(new_student->group_partner, group_partner);
    new_student->group_id = group_id;
    new_student->assignment_id = assignment_id;
    strcpy(new_student->deadline, deadline);
    strcpy(new_student->status, status);
    new_student->offline_evaluation_marks = offline_evaluation_marks;
    new_student->viva_marks = viva_marks;
    new_student->next = NULL;

    if (*student_list_ptr == NULL)
    {
        *student_list_ptr = new_student;
    }
    else
    {
        last_student = *student_list_ptr;
        while (last_student->next != NULL)
        {
            last_student = last_student->next;
        }
        last_student->next = new_student;
    }

    assignment *assignment_ptr = *assignment_list_ptr;
    while (assignment_ptr != NULL && assignment_ptr->assignment_id != assignment_id)
    {
        assignment_ptr = assignment_ptr->next;
    }

    if (assignment_id == 0)
    {
        return;
    }
    if (assignment_ptr == NULL)
    {
        assignment *new_assignment = (assignment *)malloc(sizeof(assignment));
        new_assignment->assignment_id = assignment_id;
        strcpy(new_assignment->topic_name, topic_name);
        strcpy(new_assignment->status, "declared");
        new_assignment->student_group_list = NULL;
        new_assignment->next = *assignment_list_ptr;
        *assignment_list_ptr = new_assignment;
        assignment_ptr = new_assignment;
    }

    student_group *last_student_group = assignment_ptr->student_group_list;
    if (last_student_group == NULL)
    {
        student_group *new_student_group = (student_group*)malloc(sizeof(student_group));
        new_student_group->group_id = group_id;
        strcpy(new_student_group->status, new_student->status);
        new_student_group->marks = viva_marks + offline_evaluation_marks;
        new_student_group->next = NULL;
        new_student_group->student1 = new_student;
        assignment_ptr->student_group_list = new_student_group;
    }
    else
    {
        while (last_student_group->next != NULL && last_student_group->group_id != group_id)
        {
            last_student_group = last_student_group->next;
        }
        if (last_student_group->group_id == group_id)
        {
            last_student_group->student2 = new_student;
            last_student_group->marks += (new_student->viva_marks + new_student->offline_evaluation_marks);
        }
        else if (last_student_group->next == NULL)
        {
            student_group *new_student_group = (student_group*)malloc(sizeof(student_group));
            new_student_group->group_id = group_id;
            strcpy(new_student_group->status, new_student->status);
            new_student_group->marks = viva_marks + offline_evaluation_marks;
            new_student_group->next = NULL;
            new_student_group->student1 = new_student;
            last_student_group->next = new_student_group;
        }
    }
}

void insert_assignment(assignment **assignment_list, student **student_list, int assignment_id, char *topic_name, char *status, int num_students, char *deadline)

{
    assignment *curr_assign = *assignment_list;
    while (curr_assign != NULL)
    {
        if (curr_assign->assignment_id == assignment_id)
        {
            printf("Error: assignment id %d already exists in the assignment list.\n", assignment_id);
            return;
        }
        curr_assign = curr_assign->next;
    }

    assignment *new_assign = (assignment *)malloc(sizeof(assignment));
    new_assign->assignment_id = assignment_id;
    strcpy(new_assign->topic_name, topic_name);
    strcpy(new_assign->status, status);
    new_assign->student_group_list = NULL;
    new_assign->next = NULL;

    int students_assigned = 0;
    student *curr_student = *student_list;
    while (curr_student != NULL && students_assigned < num_students)
    {
        if (curr_student->assignment_id == 0)
        {
            curr_student->assignment_id = assignment_id;
            strcpy(curr_student->deadline, deadline);
            strcpy(curr_student->status, "not submitted");

            // Find or create a student group with the same group id
            student_group *curr_group = new_assign->student_group_list;
            student_group *prev_group = NULL;
            while (curr_group != NULL && curr_group->group_id != curr_student->group_id)
            {
                prev_group = curr_group;
                curr_group = curr_group->next;
            }
            if (curr_group == NULL)
            {
                // Create a new student group node
                curr_group = (student_group *)malloc(sizeof(student_group));
                curr_group->group_id = curr_student->group_id;
                strcpy(curr_group->status, "declared");
                curr_group->marks = 0;
                curr_group->student1 = curr_student;
                curr_group->student2 = NULL;
                curr_group->next = NULL;

                // Add the new student group node to the assignment's student group list
                if (prev_group == NULL)
                {
                    new_assign->student_group_list = curr_group;
                }
                else
                {
                    prev_group->next = curr_group;
                }
            }
            else if (curr_group->student2 == NULL)
            {
                // Add the current student as the second student of an existing student group
                curr_group->student2 = curr_student;
            }
            else
            {
                printf("Warning: maximum number of students per group reached for group %d.\n", curr_student->group_id);
            }

            students_assigned++;
        }
        curr_student = curr_student->next;
    }

    if (*assignment_list == NULL)
    {
        *assignment_list = new_assign;
    }
    else
    {
        curr_assign = *assignment_list;
        while (curr_assign->next != NULL)
        {
            curr_assign = curr_assign->next;
        }
        curr_assign->next = new_assign;
    }
    if (students_assigned < num_students)
    {
        printf("Error: Not enough students with assignment id 0 \n");
        printf("Only %d students are assigned with  assignment id %d\n", students_assigned, assignment_id);
        printf("\n");
    }
}

void print_records(student *student_list, assignment *assignment_list)
{
    printf("Student Records:\n");
    student *current_student = student_list;
    while (current_student != NULL)
    {
        printf("ID: %d\n", current_student->student_id);
        printf("Name: %s\n", current_student->name);
        printf("Group Partner: %s\n", current_student->group_partner);
        printf("Group ID: %d\n", current_student->group_id);
        printf("Assignment ID: %d\n", current_student->assignment_id);
        printf("Deadline: %s\n", current_student->deadline);
        printf("Status: %s\n", current_student->status);
        printf("Offline Evaluation Marks: %.2f\n", current_student->offline_evaluation_marks);
        printf("Viva Marks: %.2f\n", current_student->viva_marks);
        printf("\n");
        current_student = current_student->next;
    }

    printf("Assignment Records:\n");
    assignment *current_assignment = assignment_list;
    while (current_assignment != NULL)
    {
        printf("ID: %d\n", current_assignment->assignment_id);
        printf("Topic Name: %s\n", current_assignment->topic_name);
        printf("Status: %s\n", current_assignment->status);
        printf("\n");
        printf("Student Groups:\n");
        student_group *current_student_group = current_assignment->student_group_list;
        while (current_student_group != NULL)
        {
            printf("Group ID: %d\n", current_student_group->group_id);
            printf("Status: %s\n", current_student_group->status);
            printf("Marks: %.2f\n", current_student_group->marks);
            printf("\n");
            current_student_group = current_student_group->next;
        }
        printf("\n");
        current_assignment = current_assignment->next;
    }
}

void update_lists(student **student_list_ptr, assignment **assignment_list_ptr, int group_id, int assignment_id, float s1_offline_marks, float s1_viva_marks, float s2_offline_marks, float s2_viva_marks, char *new_status)
{
    student_group *sg = NULL;
    assignment *as = *assignment_list_ptr;
    while (as != NULL && sg == NULL)
    {
        if (as->assignment_id == assignment_id)
        {
            student_group *temp = as->student_group_list;
            while (temp != NULL && sg == NULL)
            {
                if (temp->group_id == group_id)
                {
                    sg = temp;
                }
                temp = temp->next;
            }
        }
        as = as->next;
    }

    if (sg != NULL)
    {
        strcpy(sg->status, new_status);
        student *s = sg->student1;
        while (s != NULL)
        {
            if (s->assignment_id == assignment_id && s->group_id == group_id)
            {
                strcpy(s->status, new_status);
            }
            s = s->next;
        }
        s = sg->student2;
        while (s != NULL)
        {
            if (s->assignment_id == assignment_id && s->group_id == group_id)
            {
                strcpy(s->status, new_status);
            }
            s = s->next;
        }

        if (strcmp(new_status, "evaluated") == 0)
        {
            sg->marks = s1_offline_marks + s1_viva_marks + s2_offline_marks + s2_viva_marks;
        }

        as = *assignment_list_ptr;
        int evaluated_count = 0;
        while (as != NULL)
        {
            if (as->assignment_id == assignment_id && strcmp(as->status, "evaluated") != 0)
            {
                student_group *temp = as->student_group_list;
                while (temp != NULL)
                {
                    if (strcmp(temp->status, "evaluated") == 0)
                    {
                        evaluated_count++;
                    }
                    temp = temp->next;
                }
                if (evaluated_count == 2)
                {
                    strcpy(as->status, "evaluated");
                }
            }
            as = as->next;
        }
        if (strcmp(new_status, "evaluated") == 0)
        {
            student *st = *student_list_ptr;
            while (st != NULL)
            {
                if (st->assignment_id == assignment_id && st->group_id == group_id)
                {
                    if (st == sg->student1)
                    {
                        st->offline_evaluation_marks = s1_offline_marks;
                        st->viva_marks = s1_viva_marks;
                    }
                    else if (st == sg->student2)
                    {
                        st->offline_evaluation_marks = s2_offline_marks;
                        st->viva_marks = s2_viva_marks;
                    }
                }
                st = st->next;
            }
        }
    }
}

void print_declared_but_not_evaluated_assignments(assignment *assignment_list)
{
    assignment *current_assignment = assignment_list;

    while (current_assignment != NULL)
    {
        if (strcmp(current_assignment->status, "declared") == 0)
        {
            {
                printf("Assignment ID: %d\n", current_assignment->assignment_id);
                printf("Topic Name: %s\n", current_assignment->topic_name);
                printf("Assignment Status: %s\n", current_assignment->status);
            }
        }
        current_assignment = current_assignment->next;
    }
}
int is_deadline_over(char *deadline)
{
    int deadline_day, deadline_month, deadline_year;
    sscanf(deadline, "%d-%d-%d", &deadline_day, &deadline_month, &deadline_year);

    time_t t = time(NULL);
    struct tm *current_time = localtime(&t);
    int current_day = current_time->tm_mday;
    int current_month = current_time->tm_mon + 1;
    int current_year = current_time->tm_year + 1900;

    if (deadline_year < current_year)
    {
        return 1;
    }
    else if (deadline_year == current_year && deadline_month < current_month)
    {
        return 1;
    }
    else if (deadline_year == current_year && deadline_month == current_month && deadline_day < current_day)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

void print_student_groups_not_submitted(struct assignment *assignment_list, struct student *student_list)
{
    struct assignment *current_assignment = assignment_list;
    while (current_assignment != NULL)
    {
        printf("Students groups who have not submitted %s assignment:\n", current_assignment->topic_name);
        struct student_group *current_group = current_assignment->student_group_list;
        while (current_group != NULL)
        {
            if (strcmp(current_group->status, "notsubmitted") == 0)
            {
                struct student *student1 = current_group->student1;
                struct student *student2 = current_group->student2;
                int is_deadline_over_for_both = is_deadline_over(student1->deadline) && is_deadline_over(student2->deadline);
                if (is_deadline_over_for_both)
                {
                    printf("Group ID: %d\n", current_group->group_id);
                }
            }
            current_group = current_group->next;
        }
        current_assignment = current_assignment->next;
    }
}

void print_pending_evaluation_students(assignment *assignment_list, student *student_list)
{
    assignment *current_assignment = assignment_list;
    while (current_assignment != NULL)
    {
        printf("Assignment ID: %d\n", current_assignment->assignment_id);
        printf("Topic: %s\n", current_assignment->topic_name);
        printf("Status: %s\n", current_assignment->status);
        student_group *current_group = current_assignment->student_group_list;
        while (current_group != NULL)
        {
            if (strcmp(current_group->status, "submitted") == 0)
            {
                printf("Pending evaluations:\n");
                printf("\tGroup ID: %d\n", current_group->group_id);
                printf("\tStatus: %s\n", current_group->status);
                printf("\tAssignment ID: %d\n", current_assignment->assignment_id);
            }
            else if (strcmp(current_group->status, "evaluated") == 0)
            {
                student *student1 = current_group->student1;
                student *student2 = current_group->student2;
                int viva_remaining = 0;
                if (student1->viva_marks == -1)
                {
                    printf("\tViva remaining for student %s (ID: %d)\n", student1->name, student1->student_id);
                    viva_remaining = 1;
                }
                if (student2 != NULL && student2->viva_marks == -1)
                {
                    printf("\tViva remaining for student %s (ID: %d)\n", student2->name, student2->student_id);
                    viva_remaining = 1;
                }
                if (viva_remaining)
                {
                    printf("\tViva is remaining for group %d\n", current_group->group_id);
                }
            }
            current_group = current_group->next;
        }
        current_assignment = current_assignment->next;
    }
}

void print_student_groups_by_marks(assignment *assignment_list, int assignment_id)
{
    assignment *current_assignment = assignment_list;
    while (current_assignment != NULL && current_assignment->assignment_id != assignment_id)
    {
        current_assignment = current_assignment->next;
    }
    if (current_assignment == NULL)
    {
        printf("Assignment not found.\n");
        return;
    }
    student_group *current_group = current_assignment->student_group_list;
    int num_groups = 0;
    while (current_group != NULL)
    {
        num_groups++;
        current_group = current_group->next;
    }
    float marks[num_groups];
    current_group = current_assignment->student_group_list;
    int i = 0,j;
    while (current_group != NULL)
    {
        marks[i] = current_group->marks;
        i++;
        current_group = current_group->next;
    }
    for (i = 0; i < num_groups - 1; i++)
    {
        for ( j = i + 1; j < num_groups; j++)
        {
            if (marks[i] < marks[j])
            {
                float temp = marks[i];
                marks[i] = marks[j];
                marks[j] = temp;
            }
        }
    }
    printf("Student groups for assignment %d, sorted by marks:\n", assignment_id);
    for (i = 0; i < num_groups; i++)
    {
        current_group = current_assignment->student_group_list;
        while (current_group != NULL)
        {
            if (current_group->marks == marks[i])
            {
                printf("Group ID: %d, Status: %s, Marks: %.2f\n",
                       current_group->group_id, current_group->status, current_group->marks);
                break;
            }
            current_group = current_group->next;
        }
    }
}

void print_top_groups_for_each_assignment(assignment *assignment_list)
{
    assignment *current_assignment = assignment_list;
    while (current_assignment != NULL)
    {
        printf("Top groups for assignment %d - %s:\n", current_assignment->assignment_id, current_assignment->topic_name);
        student_group *current_group = current_assignment->student_group_list;
        float highest_marks = -1.0;
        student_group *top_group = NULL;
        while (current_group != NULL)
        {
            if (strcmp(current_group->status, "evaluated") == 0)
            {
                if (current_group->marks > highest_marks)
                {
                    highest_marks = current_group->marks;
                    top_group = current_group;
                }
            }
            current_group = current_group->next;
        }
        if (top_group != NULL)
        {
            printf("Group %d -: %.2f marks\n", top_group->group_id, top_group->marks);
        }
        else
        {
            printf("No group has been evaluated for this assignment.\n");
        }
        current_assignment = current_assignment->next;
    }
}

int main()
{   student *student_list = NULL;
    assignment *assignment_list = NULL;
 int choice, student_id, group_id, assignment_id;
 char name[MAX], group_partner[MAX], deadline[MAX], status[MAX], topic_name[MAX], event_type[MAX],new_status[MAX];
 float offline_evaluation_marks, viva_marks, marks;
 int num_students;
 float s1_offline_marks, s1_viva_marks, s2_offline_marks, s2_viva_marks;
 
 int line=0, read;
 FILE* f_ptr;
FILE* f_ptr1;
f_ptr=fopen("input.txt","r");
do {
	read= fscanf(f_ptr,"%d,%99[^,],%99[^,],%d,%d,%99[^,],%99[^,],%f,%f,%s", &student_id, name, group_partner, 
&group_id, &assignment_id, deadline, status, &offline_evaluation_marks, &viva_marks, topic_name);

line++;


if (read != 10 && !feof(f_ptr))
    	{
      		printf("\nline no = %d , File format incorrect.\n", line);    		
    	}
    
    	if (ferror(f_ptr))
    	{
      		printf("Error reading file.\n");
    	}

         insert_student_record(&student_list, &assignment_list, student_id, name, group_partner, group_id, assignment_id, deadline, status, offline_evaluation_marks, viva_marks, topic_name);

} while(!feof(f_ptr) && read==10);


password_login();
 do
    {
        printf("Menu\n");
        printf("1. Add student record\n");
        printf("2. Add assignment\n");
        printf("3. Change status\n");
        printf("4. Print declared but not evaluated assignments\n");
        printf("5. Print student records\n");
        printf("6. Print pending evaluation students\n");
        printf("7. Print student groups by marks\n");
        printf("8. Print top groups for each assignment\n");
        printf("9. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("Enter student ID: ");
            scanf("%d", &student_id);
            printf("Enter student name: ");
            scanf("%s", name);
            printf("Enter group partner: ");
            scanf("%s", group_partner);
            printf("Enter group ID: ");
            scanf("%d", &group_id);
            printf("Enter assignment ID: ");
            scanf("%d", &assignment_id);
            printf("Enter deadline: ");
            scanf("%s", deadline);
            printf("Enter status: ");
            scanf("%s", status);
            printf("Enter offline evaluation marks: ");
            scanf("%f", &offline_evaluation_marks);
            printf("Enter viva marks: ");
            scanf("%f", &viva_marks);
            printf("Enter topic name: ");
            scanf("%s", topic_name);
            insert_student_record(&student_list, &assignment_list, student_id, name, group_partner, group_id, assignment_id, deadline, status, offline_evaluation_marks, viva_marks, topic_name);
            break;
        case 2:
            printf("Enter assignment ID: ");
            scanf("%d", &assignment_id);
            printf("Enter topic name: ");
            scanf("%s", topic_name);
            printf("Enter status: ");
            scanf("%s", status);
            printf("Enter deadline: ");
            scanf("%s", deadline);
             printf("No of students: ");
            scanf("%d", &num_students);
            insert_assignment(&assignment_list, &student_list,assignment_id, topic_name, status,num_students, deadline);
            break;
        case 3:
            printf("Enter assignment ID: ");
            scanf("%d", &assignment_id);
            printf("Enter group ID: ");
            scanf("%d", &group_id);
            printf("Enter status: ");
            scanf("%s", new_status);
            if (strcmp(new_status, "evaluated") == 0)
            {
                printf("Enter marks student 1 offline marks: ");
                scanf("%f", &s1_offline_marks);
                printf("Enter marks student 1 Viva Marks: ");
                scanf("%f", &s1_viva_marks);
                printf("Enter marks student 2 offline marks: ");
                scanf("%f", &s2_offline_marks);
                printf("Enter marks student 2 Viva Marks: ");
                scanf("%f", &s2_viva_marks);
            }
            else
            {
                marks = 0;
            }
            update_lists(&student_list,&assignment_list, group_id,assignment_id, s1_offline_marks,s1_viva_marks, s2_offline_marks,  s2_viva_marks, new_status);
            break;
        case 4:
            print_declared_but_not_evaluated_assignments(assignment_list);
            break;
        case 5:
            print_records(student_list,assignment_list);
            break;
        case 6:
            print_pending_evaluation_students(assignment_list,student_list);
            break;
        case 7:
            printf("Enter assignment ID: ");
            scanf("%d", &assignment_id);
            print_student_groups_by_marks(assignment_list, assignment_id);
            break;
        case 8:
            print_top_groups_for_each_assignment(assignment_list);
            break;
        case 9:
            printf("Exiting program...");
            break;
        default:
            printf("Invalid choice, please enter a valid choice.\n");
        }
    } while (choice != 10);
}
