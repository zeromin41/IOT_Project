#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdint.h>

#define dot "/dev/dot"       // Dot Matrix
#define clcd "/dev/clcd"     // Character LCD
#define tact "/dev/tactsw"   // Tact Switch
#define FND_DEVICE "/dev/fnd"
#define DIPSW_DEVICE "/dev/dipsw"

#define MAX_CLCD_BYTES 64    // CLCD에 쓰는 최대 바이트 수
#define DOT_MATRIX_SIZE 8
#define CHARACTER_WIDTH 2
#define CHARACTER_HEIGHT 2
#define GAME_SPEED_SLOW 1000      // ms
#define GAME_SPEED_MEDIUM 500   // ms
#define GAME_SPEED_FAST 100      // ms

const unsigned char fnd_data[16] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8,
    0x80, 0x90, 0xA0, 0x88, 0x83, 0xC6, 0xA1, 0x8E
};

int dot_fd;
unsigned char dot_matrix[DOT_MATRIX_SIZE];
int character_pos_x, character_pos_y;
int obstacle_pos_x, obstacle_height;
int obstacle2_pos_x, obstacle2_height;
int is_jumping;
int jump_height;
int jump_duration;
int game_state;  // 0: Ready, 1: Play, 2: Game Over
int elapsed_time;
time_t start_time;  // 시작 시간과 종료 시간 변수 추가
time_t end_time;
int GAME_SPEED;

// 입출력 함수
void print_clcd(const char *message) {
    int clcd_fd = open(clcd, O_RDWR);
    if (clcd_fd < 0) {
        perror("CLCD open");
        exit(1);
    }
    write(clcd_fd, message, strnlen(message, MAX_CLCD_BYTES));
    close(clcd_fd);
}

void display_gameover_on_dot_matrix() {
    unsigned char gameover_matrix[DOT_MATRIX_SIZE] = {
        0b00111100,
        0b01000010,
        0b10100101,
        0b10000001,
        0b10100101,
        0b10011001,
        0b01000010,
        0b00111100
    };

    write(dot_fd, gameover_matrix, sizeof(gameover_matrix));

    usleep(2000000);  // 3초 동안 대기 (3000000 마이크로초 = 3초)

    // Dot Matrix 초기화
    memset(dot_matrix, 0, sizeof(dot_matrix));
    write(dot_fd, dot_matrix, sizeof(dot_matrix));
}

void init_game() {
    // Initialize devices
    print_clcd("Press switch to start");

    dot_fd = open(dot, O_WRONLY);
    if (dot_fd < 0) {
        perror("Dot Matrix open");
        exit(1);
    }

    // dipswitch 초기화
    int dipsw_fd = open(DIPSW_DEVICE, O_RDONLY);
    if (dipsw_fd < 0) {
        perror("Dipswitch open");
        exit(1);
    }

    unsigned char dipsw_state;
    read(dipsw_fd, &dipsw_state, sizeof(dipsw_state));
    close(dipsw_fd);

    if ((dipsw_state & 0x03) == 0x01) {
        GAME_SPEED = GAME_SPEED_MEDIUM;
    } else if ((dipsw_state & 0x03) == 0x03) {
        GAME_SPEED = GAME_SPEED_FAST;
    } else {
        GAME_SPEED = GAME_SPEED_SLOW;
    }

    // 게임 초기화
    character_pos_x = 1;
    character_pos_y = DOT_MATRIX_SIZE - CHARACTER_HEIGHT;
    obstacle_pos_x = DOT_MATRIX_SIZE - 1;
    obstacle_height = rand() % (DOT_MATRIX_SIZE - CHARACTER_HEIGHT - 2) + 1;
    obstacle2_pos_x = 0;
    obstacle2_height = rand() % (DOT_MATRIX_SIZE - 2) + 1;
    is_jumping = 0;
    jump_height = 0;
    jump_duration = 0;
    elapsed_time = 0;
    game_state = 0;
}

void start_game() {
    game_state = 1;

    // 게임 변수 초기화
    character_pos_x = 1;
    character_pos_y = DOT_MATRIX_SIZE - CHARACTER_HEIGHT;
    obstacle_pos_x = DOT_MATRIX_SIZE - 1;
    obstacle_height = rand() % (DOT_MATRIX_SIZE - CHARACTER_HEIGHT - 2) + 1;
    obstacle2_pos_x = 0;
    obstacle2_height = rand() % (DOT_MATRIX_SIZE - 2) + 1;
    is_jumping = 0;
    jump_height = 0;
    jump_duration = 0;
    elapsed_time = 0;

    start_time = time(NULL);  // 게임 시작 시간 기록
}

void end_game() {
    game_state = 2;

    display_gameover_on_dot_matrix();
    close(dot_fd);
    print_clcd("game over");
    
    end_time = time(NULL);  // 게임 종료 시간 기록
    int game_duration = (int)difftime(end_time, start_time);  // 게임 지속 시간 계산

    int fnd_d;
    unsigned char fnd_value[4] = {0};

    if ((fnd_d = open(FND_DEVICE, O_RDWR)) < 0) {
        perror("open");
        return;
    }

    while (1) {
        fnd_value[0] = fnd_data[(game_duration / 1000) % 10]; // 천의 자리
        fnd_value[1] = fnd_data[(game_duration / 100) % 10]; // 백의 자리
        fnd_value[2] = fnd_data[(game_duration / 10) % 10]; // 십의 자리
        fnd_value[3] = fnd_data[game_duration % 10]; // 일의 자리
        write(fnd_d, fnd_value, sizeof(fnd_value));
        usleep(5000000); // 1초 대기
    }

    close(fnd_d);
}

void jump_character() {
    if (is_jumping) {
        character_pos_y -= jump_height;
        jump_duration--;

        // 캐릭터가 화면 밖으로 벗어나지 않게 조정
        if (character_pos_y < 0) {
            character_pos_y = 0;
            is_jumping = 0;
        }
        else if (jump_duration == 0) {
            is_jumping = 0;
        }
    }
    else {
        if (character_pos_y < DOT_MATRIX_SIZE - CHARACTER_HEIGHT) {
            character_pos_y++;
        }
    }
}

void move_obstacle() {
    obstacle_pos_x--;

    if (obstacle_pos_x < 0) {
        obstacle_pos_x = DOT_MATRIX_SIZE - 1;
        obstacle_height = rand() % (DOT_MATRIX_SIZE - CHARACTER_HEIGHT - 2) + 1;
    }

    obstacle2_pos_x++;
    if (obstacle2_pos_x >= DOT_MATRIX_SIZE) {
        obstacle2_pos_x = 0;
        obstacle2_height = rand() % (DOT_MATRIX_SIZE - 2) + 1;
    }
}

void update_dot_matrix() {
    memset(dot_matrix, 0, sizeof(dot_matrix));

    // 캐릭터 그리기
    for (int i = 0; i < CHARACTER_HEIGHT; i++) {
        for (int j = 0; j < CHARACTER_WIDTH; j++) {
            if (character_pos_y + i < DOT_MATRIX_SIZE) {
                dot_matrix[character_pos_y + i] |= 1 << (character_pos_x + j);
            }
        }
    }

    // 장애물 그리기
    for (int i = 0; i < obstacle_height; i++) {
        if (obstacle_pos_x >= 0 && obstacle_pos_x < DOT_MATRIX_SIZE) {
            dot_matrix[DOT_MATRIX_SIZE - 1 - i] |= 1 << obstacle_pos_x;
        }
    }

    // 상단 장애물 그리기
    for (int i = 0; i < obstacle2_height; i++) {
        if (obstacle2_pos_x >= 0 && obstacle2_pos_x < DOT_MATRIX_SIZE) {
            dot_matrix[i] |= 1 << obstacle2_pos_x;
        }
    }

    write(dot_fd, dot_matrix, sizeof(dot_matrix));
}

int is_collision() {
    for (int i = 0; i < CHARACTER_HEIGHT; i++) {
        if (obstacle_pos_x <= character_pos_x + CHARACTER_WIDTH - 1 &&
            obstacle_pos_x >= character_pos_x &&
            character_pos_y + i >= DOT_MATRIX_SIZE - obstacle_height) {
            return 1;
        }
        if (obstacle2_pos_x == character_pos_x &&
            character_pos_y + i < obstacle2_height) {
            return 1;
        }
    }
    return 0;
}

void check_buttons() {
    int tact_fd = open(tact, O_RDWR);
    if (tact_fd < 0) {
        perror("Tact Switch open");
        exit(1);
    }

    unsigned char tact_state;
    read(tact_fd, &tact_state, sizeof(tact_state));

    if (tact_state & 0x01) {  // Start button
        if (game_state == 0) {
            start_game();
        }
    }

    if (tact_state & 0x02) {  // Jump button
        if (game_state == 1 && !is_jumping) {
            is_jumping = 1;
            jump_height = 3;  // 점프 높이 조절
            jump_duration = 4;  // 점프 지속 시간 조절
        }
    }

    close(tact_fd);
}

void wait_for_start() {
    while (game_state == 0) {
        check_buttons();
        usleep(100000);  // 100ms 대기
    }
}

void game_loop() {
    while (1) {
        if (game_state == 1) {
            update_dot_matrix();
            move_obstacle();
            jump_character();

            if (is_collision()) {
                end_game();
            }

            elapsed_time++;
            usleep(GAME_SPEED * 1000);  // ms to us
        }
        else if (game_state == 2) {
            break;
        }

        check_buttons();
    }
}

int main() {
    init_game();
    wait_for_start();
    game_loop();

    return 0;
}