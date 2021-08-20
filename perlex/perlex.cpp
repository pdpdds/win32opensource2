#include <EXTERN.h>
#include <perl.h>

// 정적 펄 인터프리터 객체 선언
static PerlInterpreter * my_perl;
int main(int argc, char** argv, char** env)
{
    //펄 스크립트 시스템을 초기화하고 펄 인터프리터 객체를 생성한다.
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    // Add100.pl을 명령 인자로 넘겨서 스크립트를 파싱한다.
    perl_parse(my_perl, NULL, argc, argv, (char**)NULL);
    //스크립트를 실행한다.
    perl_run(my_perl);
    //펄 인터프리터 객체를 해제한다.
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();

    return 0;
}