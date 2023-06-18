#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* Structures to represent contributors and projects */
struct section {
    char* name;
    int N;
    char** skills;
    int* levels;
};
struct SkillLevel {
    char* skill;
    int level;
};
struct project {
    char* name;
    int number_of_days;
    int score;
    int best_before;
    int roles;
    char** skills;
    int* level;
};
//this function provide the sort of contributer based on their ASCII value
int compareSkills(const void* a, const void* b) {
    return strcmp(*(const char**)a, *(const char**)b);
}

/* Function to compare projects based on their score/number_of_days ratio */
int compareProjects(const void* a, const void* b) {
  const struct project* projA = (const struct project*)a;
  const struct project* projB = (const struct project*)b;
  
  double para1 = (double)projA->score / projA->number_of_days;
  double para2 = (double)projB->score / projB->number_of_days;
  
  if (para1 > para2) {
    return -1;
  } else if (para1 < para2) {
    return 1;
  } else {
    return 0;
  }
}
//this function is to sort the contributers based on their availability by comparing the number of skills the first is the one who has a high number of skills
int availability(const void* a, const void* b) {
  const struct section* size1 = (const struct section*)a;
  const struct section* size2 = (const struct section*)b;
  int max_value = (size1->N > size2->N) ? size1->N : size2->N;
  return max_value;
}
/* Binary search function to find a target in an array */
int binary_search(char** array, char* target, int start, int end) {
  while (start <= end) {
    int middle = start + (end-start) / 2;
    int comp = strcmp(target, array[middle]);
    if (comp == 0) {
      return middle;
    } else if (comp < 0) {
      end = middle - 1;
    } else {
      start = middle + 1;
    }
  }
  return -1;
}
/* Function to handle mentorship and assign contributors to projects */
char* mentorship(struct section* sections, char* contributor, char* skill, int level_skill, int C, int number_skill, struct project* projects) {
  for (int i = 0; i < C; i++) {
    struct section* A = &sections[i];
    int cnt = binary_search(A->skills, skill, 0, A->N - 1);
    if (cnt != -1 && A->levels[cnt] == level_skill + 1) {
      for (int j = 0; j < number_skill; j++) {
	int index = binary_search(A->skills, projects->skills[j], 0, A->N - 1);
	if (index != -1 && index != cnt && A->levels[index] >= projects->level[j]) {
	  return contributor;
	} else {
	  return A->name;
	}
      }
    }
  }
  return NULL;
}
/* Function to increment skill level for the mentor */
void* getSkill(struct section* sections,char* skill, int C){
  for (int i = 0; i < C; i++) {
    struct section* A = &sections[i];
    int cnt = binary_search(A->skills, skill, 0, A->N - 1);
    A->levels[cnt]+=1;
  }
}
/* Structure to store dynamically growing maximum scores */
struct dynamic{
  int *max;
  int P;
};
/* Structure to store project assignments */
struct assign{
  char* project;
  char** cont;
};

int count=0;
void append(struct dynamic *A,int data){
  A->P++;
  A->max=realloc(A->max,A->P*sizeof(int));
  A->max[A->P-1]=data;
  count++;
  if(data>A->P){
    A->max=realloc(A->max, 2*A->P*sizeof(int));
    A->max[A->P]=data;
    count+=1;
  }
}

int main() {
  FILE* file_object = fopen("input.txt", "r");
  if (file_object == NULL) {
    printf("Erreur!");
    return 1;
  }
  
  int C, P;
  fscanf(file_object, "%d %d", &C, &P);
  struct section* sections = malloc(C * sizeof(struct section));
  struct project* projects = malloc(P * sizeof(struct project));
  // Reading and initializing sections
  for (int i = 0; i <C; i++) {
    struct section* A = &sections[i];
    A->name =(char*) malloc(21 * sizeof(char));
    fscanf(file_object, "%s %d", A->name, &A->N);
    A->skills =(char**) malloc((A->N) * sizeof(char*));
    A->levels =(int*) malloc((A->N) * sizeof(int));
    for (int j = 0; j < A->N; j++) {
      A->skills[j] =(char*) malloc(21 * sizeof(char));
      fscanf(file_object, "%s", A->skills[j]);
      fscanf(file_object, "%d", &A->levels[j]);
      }
    // Sorting skills based on levels
    struct SkillLevel* skillLevels = malloc(A->N * sizeof(struct SkillLevel));
    for (int k = 0; k < A->N; k++) {
      skillLevels[k].skill = A->skills[k];
      skillLevels[k].level = A->levels[k];
      }
    
    qsort(skillLevels, A->N, sizeof(struct SkillLevel), compareSkills);
    
    char** sortedSkills = malloc(A->N * sizeof(char*));
    int* sortedLevels = malloc(A->N * sizeof(int));
    for (int l = 0; l < A->N; l++) {
      A->skills[l] = skillLevels[l].skill;
      A->levels[l] = skillLevels[l].level;
      }
  }
  // Reading and initializing projects
  for (int i = 0; i < P; i++) {
    struct project* Pjts = &projects[i];
    Pjts->name = (char*)malloc(21 * sizeof(char));
    fscanf(file_object, "%s %d %d %d %d", Pjts->name, &Pjts->number_of_days, &Pjts->score, &Pjts->best_before, &Pjts->roles);
    Pjts->skills =(char**) malloc(Pjts->roles * sizeof(char*));
    Pjts->level = (int*)malloc(Pjts->roles * sizeof(int));
    for (int j = 0; j < Pjts->roles; j++) {
      Pjts->skills[j] = (char*)malloc(21 * sizeof(char));
      fscanf(file_object, "%s", Pjts->skills[j]);
      fscanf(file_object, "%d", &Pjts->level[j]);
	 }
  }
  fclose(file_object);
  FILE *output=fopen("output.txt","w");
  if(output==NULL){
    printf("error\n");
    return 1;
  }
  struct dynamic B;
  B.max = NULL;
  B.P = 0;
  int cnt_availability[C];
  for (int p = 0; p < C; p++) {
    cnt_availability[p] = -1;
  }
  // Sorting projects
  struct assign* L = malloc(P * sizeof(struct assign));
  qsort(projects, P, sizeof(struct project), compareProjects);
  // Sorting sections based on availability
  qsort(sections, C, sizeof(struct section), availability);
  int i = 0;
  int score;
  int number_of_projects_worked=0;
  while (i < P) {
    struct project* our_proj = &projects[i];
    L[i].project = our_proj->name;
    L[i].cont = NULL; 
    int roleAssigned = 0;
    int projectWorkable = 1;

    for (int j = 0; j < our_proj->roles; j++) {
      int skillCanBeWorked = 0;
      int canWorkOnPrevious = 0;
      // Iterate over all sections to find contributors with matching skills and levels
      for (int k = 0; k < C; k++) {
	struct section* A = &sections[k];
	int prm = binary_search(A->skills, our_proj->skills[j], 0, A->N - 1);
	if (prm != -1) {
	  if (A->levels[prm] >= our_proj->level[j]) {
	    if (cnt_availability[k] != -1) {
	      struct project* prev_proj = &projects[cnt_availability[k]];
	      // Check if the previous project has fewer days than the current one
	      if (prev_proj->number_of_days < our_proj->number_of_days) {
		canWorkOnPrevious = 1;
		}
	      }
	    // Assign the contributor to the current project if they are available
	    if (cnt_availability[k] == -1 || canWorkOnPrevious) {
	      roleAssigned++;
	      L[i].cont = realloc(L[i].cont, (roleAssigned + 1) * sizeof(char*));
	      L[i].cont[roleAssigned - 1] = malloc(21 * sizeof(char));
	      strcpy(L[i].cont[roleAssigned - 1], A->name);
	      cnt_availability[k] = i;
	      skillCanBeWorked = 1;
	      }
	    } else {
	    // If the contributor's skill level is not sufficient, find a mentor
	    char* mentor = mentorship(sections, A->name, our_proj->skills[j], our_proj->level[j], C, our_proj->roles, our_proj);
	    // If a mentor is found and it matches the current section, assign the contributor
	    if (mentor != NULL && strcmp(mentor, A->name) == 0 && cnt_availability[k] == -1) {
	      cnt_availability[k] = i;
	      roleAssigned++;
	      L[i].cont = realloc(L[i].cont, (roleAssigned + 1) * sizeof(char*));
	      L[i].cont[roleAssigned - 1] = malloc(21 * sizeof(char));
	      strcpy(L[i].cont[roleAssigned - 1], A->name);
	      A->levels[prm] += 1;
	      break;
	      }
	    }
	  }
      }
    }
    // If the project is workable and all roles are assigned, process the project
    if (projectWorkable && roleAssigned == our_proj->roles) {
      fprintf(output,"%s ", our_proj->name);
      number_of_projects_worked+=1;
      score = our_proj->score + our_proj->best_before - our_proj->number_of_days;
      if (score > 0) {
	score = score;
      } else {
	score = 0;
      }
      append(&B, score);
      // Print the assigned contributors for the current project
      for (int c = 0; c < roleAssigned; c++) {
	if (L[i].cont[c] != NULL && L[i].cont[c][0] != '\0') {
	  fprintf(output," %s ", L[i].cont[c]);
	  }
      }
      fprintf(output,"\n");
    } else {
      // If the project is not workable or roles are not assigned, reset the availability for the contributors
      for (int p = 0; p < C; p++) {
	if (cnt_availability[p] == i) {
	  cnt_availability[p] = -1;
	  }
      }
    }
    
    i++;
  }
  fprintf(output,"%d\n",number_of_projects_worked);
  int max_score = 0;
  for (int h = 0; h < B.P; h++) {
    max_score += B.max[h];
  }
  return 0;
}
