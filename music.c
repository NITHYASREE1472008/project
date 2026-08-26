#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Song {
    char path[200];
    struct Song *prev;
    struct Song *next;
};

struct Song *head    = NULL;
struct Song *current = NULL;

// ── Stop any playing song ──
void stopSong() {
    system("killall afplay 2>/dev/null");
}

// ── Play function (Fixed: uses afplay for Mac) ──
void playSong(char path[]) {
    stopSong();  // stop previous song first
    char command[300];
    sprintf(command, "afplay \"%s\" &", path);  // & plays in background
    system(command);
}

// ── Add song ──
void addSong(char path[]) {
    struct Song *newSong = malloc(sizeof(struct Song));
    strcpy(newSong->path, path);
    newSong->next = NULL;
    newSong->prev = NULL;

    if (head == NULL) {
        head    = newSong;
        current = newSong;
    } else {
        struct Song *temp = head;
        while (temp->next != NULL)
            temp = temp->next;
        temp->next    = newSong;
        newSong->prev = temp;
    }

    printf("Song added: %s\n", path);
}

// ── Display playlist ──
void display() {
    struct Song *temp = head;
    if (!temp) { printf("Playlist empty\n"); return; }

    int i = 1;
    printf("\n===== PLAYLIST =====\n");
    while (temp != NULL) {
        char marker = (temp == current) ? '>' : ' ';
        printf(" %c %d. %s\n", marker, i++, temp->path);
        temp = temp->next;
    }
    printf("====================\n");
    printf("  '>' = current song\n");
}

// ── Play current ──
void play() {
    if (current != NULL) {
        printf("Now Playing: %s\n", current->path);
        playSong(current->path);
    } else {
        printf("No song selected\n");
    }
}

// ── Next ──
void next() {
    if (current && current->next) {
        current = current->next;
        play();
    } else {
        printf("No next song\n");
    }
}

// ── Previous ──
void previous() {
    if (current && current->prev) {
        current = current->prev;
        play();
    } else {
        printf("No previous song\n");
    }
}

// ── Delete ──
void deleteSong(char path[]) {
    struct Song *temp = head;
    while (temp) {
        if (strcmp(temp->path, path) == 0) {
            if (temp->prev) temp->prev->next = temp->next;
            else            head = temp->next;
            if (temp->next) temp->next->prev = temp->prev;
            if (current == temp)
                current = temp->next ? temp->next : temp->prev;
            free(temp);
            printf("Deleted: %s\n", path);
            return;
        }
        temp = temp->next;
    }
    printf("Song not found\n");
}

// ── Search ──
void searchSong(char path[]) {
    struct Song *temp = head;
    while (temp) {
        if (strcmp(temp->path, path) == 0) {
            printf("Found: %s\n", temp->path);
            return;
        }
        temp = temp->next;
    }
    printf("Not found\n");
}

// ── Sort ──
void sortSongs() {
    struct Song *i, *j;
    char temp[200];
    for (i = head; i && i->next; i = i->next) {
        for (j = i->next; j; j = j->next) {
            if (strcmp(i->path, j->path) > 0) {
                strcpy(temp,   i->path);
                strcpy(i->path, j->path);
                strcpy(j->path, temp);
            }
        }
    }
    printf("Sorted!\n");
}

// ════════════════════════════════════════
//  MAIN
// ════════════════════════════════════════
int main() {
    // ── Preloaded songs (your actual files) ──
    addSong("song1.mp3");
    addSong("song2.mp3");
    addSong("song3.mp3");
    addSong("song4.mp3");

    int  choice;
    char path[200];

    printf("\nWelcome to Music Player!\n");

    while (1) {
        printf("\n--- MUSIC PLAYER ---\n");
        printf("1. Add Song\n");
        printf("2. Display\n");
        printf("3. Play\n");
        printf("4. Next\n");
        printf("5. Previous\n");
        printf("6. Stop\n");
        printf("7. Delete\n");
        printf("8. Search\n");
        printf("9. Sort\n");
        printf("0. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Enter song filename (e.g., song5.mp3): ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = 0;
                addSong(path);
                break;
            case 2: display();  break;
            case 3: play();     break;
            case 4: next();     break;
            case 5: previous(); break;
            case 6: stopSong(); printf("Stopped.\n"); break;
            case 7:
                printf("Enter filename to delete: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = 0;
                deleteSong(path);
                break;
            case 8:
                printf("Enter filename to search: ");
                fgets(path, sizeof(path), stdin);
                path[strcspn(path, "\n")] = 0;
                searchSong(path);
                break;
            case 9: sortSongs(); break;
            case 0: stopSong(); printf("Goodbye!\n"); exit(0);
            default: printf("Invalid choice\n");
        }
    }
}