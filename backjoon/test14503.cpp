/*
g++ test14503.cpp -o test14503.exe 후 F5 실행
*/
#include <iostream>
#include <queue>
using namespace std;

// 방향 배열 (북, 동, 남, 서 순서)
// dr: 행 변화량, dc: 열 변화량
// 왜 {-1,1,0,0} / {0,0,-1,1} 쓰면 안되는가? 
// -> 문제 조건에서 d=0은 북, d=1은 동, d=2는 남, d=3은 서로 정의됨
//    따라서 북→동→남→서 순서와 맞지 않으면 로봇이 엉뚱한 방향으로 움직임
int dr[4] = { -1, 0, 1, 0 }; // 북, 동, 남, 서
int dc[4] = { 0, 1, 0, -1 };

int N, M;          // 지도 크기
int cnt = 0;       // 청소 칸 수 (사용 안됨, ans 사용)
int map[51][51];   // 지도 정보: 0=빈칸, 1=벽
bool visit[51][51];// 청소 여부

// 로봇 상태 구조체: 행, 열, 방향
struct state {
    int r, c, d;
};

int main() {
    int r, c, d;  // 로봇 초기 위치와 방향
    cin >> N >> M;

    // 로봇 초기 위치와 방향 입력
    cin >> r >> c >> d;

    // 지도 입력
    // i=행, j=열 순서
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            cin >> map[i][j];
        }
    }

    queue<state> q;       // BFS 큐
    q.push({ r, c, d });  // 초기 상태 넣기
    int ans = 0;          // 청소한 칸 수

    while (!q.empty()) {
        state cur = q.front(); // 현재 로봇 상태
        q.pop();
        int r = cur.r;
        int c = cur.c;
        int d = cur.d;

        // 현재 칸 청소
        if (!visit[r][c]) {
            visit[r][c] = true;
            ans++;          // 청소 칸 수 증가
        }

        // 주변 4칸 중 청소 가능한 곳 있는지 확인
        bool hasdirty = false;
        for (int i = 0; i < 4; i++) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            // 지도 범위를 벗어나면 무시
            if (nr < 0 || nr >= N || nc < 0 || nc >= M) continue;

            // 청소 안 된 빈칸이 있으면 true
            if (!visit[nr][nc] && map[nr][nc] == 0) {
                hasdirty = true;
            }
        }

        // 주변에 청소할 곳이 없으면 후진
        if (!hasdirty) {
            int backDir = (d + 2) % 4; // 후진 방향
            int br = r + dr[backDir];
            int bc = c + dc[backDir];

            // 후진하려는 칸이 지도 밖이면 종료
            if (br < 0 || br >= N || bc < 0 || bc >= M) break;

            // 벽이면 종료
            if (map[br][bc] == 1) {
                break;
            }

            // 후진 가능하면 그대로 방향 유지 후 이동
            q.push({ br, bc, d });
            continue; // 다음 루프 진행
        }

        // 주변에 청소할 곳이 있으면 왼쪽으로 회전
        int nd = (d + 3) % 4; // 반시계 회전 (왼쪽 90도)

        int nr = r + dr[nd];
        int nc = c + dc[nd];

        // 전진 가능한지 한 번에 체크
        // 조건: 지도 안, 벽 아님, 미청소
        if (nr >= 0 && nr < N && nc >= 0 && nc < M && map[nr][nc] == 0 && !visit[nr][nc]) {
            q.push({ nr, nc, nd }); // 전진
        }
        else {
            q.push({ r, c, nd });   // 전진 못하면 회전만
        }
    }

    // 청소한 칸 수 출력
    cout << ans << endl;

    return 0;
}
