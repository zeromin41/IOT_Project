# 프로젝트 개요
본 IOT 프로젝트는 임베디드 시스템을 활용하여 점프 게임을 개발하는 것을 목표로 합니다. <br>
이 게임은 플레이어가 캐릭터를 조작하여 다가오는 장애물을 피하고 최대한 오래 살아남는 것이 목표입니다. <br>
게임은 Dot Matrix에 캐릭터와 장애물이 표시되며, CLCD에는 게임 상태와 게임 오버 메시지가 출력됩니다. <br>
플레이어는 Tact Switch를 사용하여 캐릭터를 점프시켜 장애물을 피할 수 있습니다. <br>
게임 속도는 Dip Switch를 통해 조절할 수 있으며, 게임 종료 시 FND에 플레이 시간이 표시됩니다.

## 개발 기간
> 2024.05 ~ 2024.06

## 팀원 소개
[김혜민](https://github.com/minixzip)
[심영민](https://github.com/zeromin41)
[정재형](https://github.com/jaehyeongjung)
[전하라](https://github.com/rarabong)

## 사용 개발 환경

<div>
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/linux/linux-original.svg" height="40" width="52" alt="linux logo" />
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/ubuntu/ubuntu-plain.svg" height="40" width="52" alt="ubuntu logo" />
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/vscode/vscode-original.svg" height="40" width="52" alt="vscode logo" />
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/github/github-original.svg" height="40" width="52" alt="github logo" />
  <img src="https://images.sftcdn.net/images/t_app-icon-m/p/25749bda-7112-4156-afe8-5de81b5f97cb/629816456/tera-term-800x600_0.png" height="40" alt="teraterm logo" />
</div>

# 플로우차트
```mermaid
flowchart TD
    A[시작] --> B[하드웨어장치초기화]
    B --> C[게임변수설정]
    C --> D[CLCD에 '시작하려면스위치를누르세요' 출력]
    D --> E{game_state == 0?}
    E -->|예| F[택트스위치입력확인]
    F -->|택트스위치눌림| G[게임시작]
    G --> H[game_state를 1로설정]
    H --> I[WHILE game_state == 1]
    I --> J[도트매트릭스업데이트]
    J --> K[장애물이동]
    K --> L[캐릭터점프확인]
    L --> M{충돌감지?}
    M -->|예| N[게임종료]
    N --> O[CLCD에 '게임오버' 출력]
    O --> P[FND에플레이시간출력]
    P --> Q[game_state를 2로설정]
    M -->|아니오| R{game_state == 2?}
    R -->|예| S[끝]
    R -->|아니오| I
    E -->|아니오| F
```

# 프로젝트 계획
<details>
<summary><h2>
  1주차 프로젝트 계획
</h2></summary>
  
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/429760d4-0c81-43b0-91dc-48dbfc69a7a2)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/67e4fcf6-7b47-4a11-9a52-d0e6fe0802a3)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/9e70f351-f81f-4ba8-87ab-d48fe13b8251)

</details>

<details>
<summary><h2>
  2주차 프로젝트 계획
</h2></summary>

![image](https://github.com/zeromin41/IOT_Project/assets/130297212/dac1363a-5915-485c-bdad-2957c179700c)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/6997b30f-21cc-4fc2-a65a-1f9756a62431)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/9e140422-5929-469d-aa34-2f35c55ddd5f)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/cd431898-9bf1-4e38-ac4b-408c193788f2)

</details>

<details>
<summary><h2>
  3주차 프로젝트 계획
</h2></summary>

![image](https://github.com/zeromin41/IOT_Project/assets/130297212/f1ed6a54-0056-4afb-8657-f2c11ed8dee7)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/bffcb6da-af90-4841-ba7e-418dff1de73e)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/6b53b3e3-1f11-43d3-abbd-bdcb7de948f6)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/9f310d85-4d44-40ca-99e9-34ce0b43f0fd)

</details>

<details>
<summary><h2>
  4주차 프로젝트 계획
</h2></summary>

![image](https://github.com/zeromin41/IOT_Project/assets/130297212/9d4e9c91-372f-4874-b77b-e2c2df891ca0)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/66b78960-c4be-495b-a4b6-31e31f2377aa)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/c0a18cb1-a5da-43a7-92d5-c9aa991d670b)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/ca3f9bd9-add6-459f-8771-3bedb7af0bfa)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/36c4bbbd-3235-443a-b73b-cb288f8b48c4)


</details>

<details>
<summary><h2>
  5주차 프로젝트 최종 발표
</h2></summary>

![image](https://github.com/zeromin41/IOT_Project/assets/130297212/fdf6035e-7012-4b65-b643-3035136bc8d7)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/e65806a0-053e-4456-8f9a-0925cebc7b1b)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/f02e5b9b-793a-498e-bd36-a50e98be7cdd)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/9b63ad2a-ef2e-4db2-b0cf-d03ac74433cc)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/3a7e72c6-3ccf-40f7-964a-4b0d5298fe45)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/dda000c7-f09e-4bd5-82ad-27d96d1e0c78)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/45c049b0-0a95-403f-a3d1-5fe76033a07c)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/37fd73c2-c395-4ca1-bebc-c103fbcc56b3)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/813f7759-a4d6-405e-8356-02fdc954f273)

</details>

# 프로젝트 시연 영상
[시연 영상 유튜브 링크](https://www.youtube.com/watch?v=nFt3l6eh_fw)
## 프로젝트 주요 화면
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/d4e1edc4-2744-4b1b-8a3a-62a40d0da9a9)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/a85ccfaa-a7ca-447e-afb7-0b343c130a9f)
![image](https://github.com/zeromin41/IOT_Project/assets/130297212/e60a7c94-e467-476d-aca3-728d1ec4e6b7)



