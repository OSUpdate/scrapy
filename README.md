# scrapy
## 개발환경
* 유닉스 환경
* c언어 기반 크롤러

## 사용법
~~~
make
./scrapy 옵션 주소
~~~

## 옵션
* -h : 헤더에 데이터 설정
* -o : http 응답 body 데이터를 저장
* -a : 해당 페이지의 링크(최대 20개) 파싱
* -p : post 방식으로 설정
* -d : post 방식으로 설정 후 body 데이터 입력가능
* -H : 옵션 설명 출력
* -L : 300번대 응답이 올 경우 리다이렉션 페이지로 재전송
* -l : body 부분 표시안함
* -s : haeder, body 부분 표시안함
