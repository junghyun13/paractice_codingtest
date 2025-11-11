/*
추가 안전 팁

터미널에서 수동으로 exe 만들기

g++ minchocomilk.cpp -o minchocomilk.exe


생성 확인 후 VS Code에서 launch.json의 preLaunchTask 잠시 제거
→ exe가 사라지지 않는지 확인

정상 확인 후 다시 preLaunchTask 활성화
.exe만들고 f5 눌러 실행
*/



#include <iostream>
#include <vector>
#include <queue>
#include <cstring>
#include <tuple>
#include <algorithm>

using namespace std;

int N, T; // N: 격자 크기, T: 시뮬레이션 일수
vector<vector<int>> F;          // 학생 음식 정보 (비트마스크)
vector<vector<int>> B;          // 학생 신앙심
vector<vector<bool>> visited;   // 저녁 전파 여부 체크
vector<pair<int, int>> parent;  // Union-Find에서 그룹 대표자 추적

// 상하좌우 방향 (i, j 좌표 변화)
int dirI[4] = {-1, 1, 0, 0};
int dirJ[4] = {0, 0, -1, 1};

// --------------------- 초기화 ---------------------
// 하루가 시작될 때 Union-Find 초기화 및 visited 초기화
void initialize() {
    parent.clear();
    parent.resize(N * N);
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            parent[N * i + j] = {i, j}; // 자기 자신을 대표자로 초기화
    
    visited = vector<vector<bool>>(N, vector<bool>(N, false));
}

// --------------------- 아침 ---------------------
// 모든 학생 신앙심 +1
void breakfast() {
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            B[i][j]++;
}

// --------------------- Union-Find ---------------------
// (i,j)의 그룹 대표자를 찾음
pair<int, int> find(int i, int j) {
    int u = N * i + j;
    if (parent[u] == make_pair(i, j)) // 자기 자신이 대표자면 반환
        return parent[u];
    return parent[u] = find(parent[u].first, parent[u].second); // 경로 압축
}

// (i,j) 그룹과 (ii,jj) 그룹 합치기
void merge(int i, int j, int ii, int jj) {
    if (find(i, j) == find(ii, jj)) return; // 이미 같은 그룹이면 종료

    auto p1 = find(i, j);
    auto p2 = find(ii, jj);
    i = p1.first; j = p1.second;
    ii = p2.first; jj = p2.second;

    // 그룹 대표자를 신앙심, 좌표 기준으로 결정
    if (B[i][j] > B[ii][jj]) {
        merge(ii, jj, i, j);
        return;
    }
    if (B[i][j] == B[ii][jj] && ii > i) {
        merge(ii, jj, i, j);
        return;
    }
    if (B[i][j] == B[ii][jj] && ii == i && jj > j) {
        merge(ii, jj, i, j);
        return;
    }

    // ii,jj를 새로운 대표자로 설정
    int u = N * i + j;
    int v = N * ii + jj;
    parent[u] = parent[v];
}

// --------------------- 점심 ---------------------
// 같은 음식을 좋아하는 학생끼리 그룹 통합
void lunch() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            for (int k = 0; k < 4; k++) { // 상하좌우 탐색
                int ii = i + dirI[k];
                int jj = j + dirJ[k];
                if (ii < 0 || ii >= N || jj < 0 || jj >= N) continue;
                if (F[i][j] == F[ii][jj])
                    merge(i, j, ii, jj); // 같은 음식이면 그룹 합치기
            }
        }
    }

    // 그룹 대표자에게 신앙심 모으기
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++) {
            pair<int, int> p = find(i, j); // 대표자 찾기
            B[i][j]--;                     // 원래 학생 신앙심 감소
            B[p.first][p.second]++;        // 대표자에게 합산
        }
}

// --------------------- 저녁 전파 ---------------------
// 한 학생이 신앙심을 주변 학생에게 전파
void convey(int i, int j) {
    int f = F[i][j];            // 전파할 음식 종류
    int k = B[i][j] % 4;        // 방향 결정 (0~3)
    int x = B[i][j] - 1;        // 남은 신앙심
    B[i][j] = 1;                // 현재 학생 신앙심 최소화

    while (true) {
        i += dirI[k];
        j += dirJ[k];
        if (i < 0 || i >= N || j < 0 || j >= N) break; // 범위 벗어나면 종료
        if (F[i][j] == f) continue;                     // 같은 음식이면 스킵

        visited[i][j] = true;                            // 전파 완료 표시

        if (x > B[i][j]) {
            F[i][j] = f;                                // 음식 변경
            x -= (B[i][j] + 1);
            B[i][j]++;
        } else {
            F[i][j] |= f;                               // 기존 음식 + 비트합
            B[i][j] += x;
            x = 0;
        }
        if (x == 0) break;                               // 전파 완료
    }
}

// --------------------- 저녁 ---------------------
// 하루 대표자 순서대로 전파
int priority[8] = {-1, 1, 1, 2, 1, 2, 2, 3};
void dinner() {
    vector<tuple<int, int, int, int>> vt;

    // 대표자만 리스트업
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            if (find(i, j) == make_pair(i, j))
                vt.push_back({priority[F[i][j]], -B[i][j], i, j});
    
    sort(vt.begin(), vt.end()); // 우선순위, 신앙심 기준 정렬

    for (auto tu : vt) {
        int i = get<2>(tu), j = get<3>(tu);
        if (visited[i][j]) continue; // 이미 전파했으면 스킵
        convey(i, j);                // 신앙심 전파
    }
}

// --------------------- 상태 출력 ---------------------
// 각 음식 조합별 신앙심 합계 출력
void print_status() {
    vector<int> vt(8, 0); // 0~7 조합별 합계
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            vt[F[i][j]] += B[i][j];
    
    // TCM, TC, TM, CM, T, C, M 순서 출력
    cout << vt[7] << " " << vt[3] << " " << vt[5] << " " << vt[6] << " "
         << vt[4] << " " << vt[2] << " " << vt[1] << "\n";
}

// --------------------- 메인 ---------------------
int main() {
    cin >> N >> T; // 입력: N, T
    F = vector<vector<int>>(N, vector<int>(N, 0));
    B = vector<vector<int>>(N, vector<int>(N, 0));

    // 음식 입력 -> 숫자/비트마스크 변환
    for (int i = 0; i < N; i++) {
        string str;
        cin >> str;
        for (int j = 0; j < N; j++) {
            if (str[j] == 'T') F[i][j] = 1;
            else if (str[j] == 'C') F[i][j] = 2;
            else if (str[j] == 'M') F[i][j] = 4;
        }
    }

    // 신앙심 입력
    for (int i = 0; i < N; i++)
        for (int j = 0; j < N; j++)
            cin >> B[i][j];

    // 시뮬레이션 T일 반복
    for (int day = 0; day < T; day++) {
        initialize();  // Union-Find 초기화
        breakfast();   // 아침
        lunch();       // 점심
        dinner();      // 저녁
        print_status();// 결과 출력
    }

    return 0;
}
