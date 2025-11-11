#include <iostream>
#include <vector>
#include <algorithm>
using namespace std; 

int N;

int main() {
    cin >> N;
    vector<pair<int, int>> time(N);
    for (int i = 0; i < N; i++) {
        cin >> time[i].first >> time[i].second; // (시작, 끝)
    }

    // ✅ 종료 시간을 기준으로 정렬 (같으면 시작 시간이 빠른 순)
    sort(time.begin(), time.end(), [](auto &a, auto &b) {
        if (a.second == b.second) return a.first < b.first;
        return a.second < b.second;
    });

    int cnt = 0;
    int end_time = 0; // 마지막으로 선택한 회의의 종료시간

    for (int i = 0; i < N; i++) {
        // ✅ 현재 회의 시작시간이 이전 회의 종료시간 이후면 선택
        if (time[i].first >= end_time) {
            cnt++;
            end_time = time[i].second;
        }
    }

    cout << cnt << endl; // ✅ 최댓값 출력

    return 0;
}
