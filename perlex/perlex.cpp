#include <EXTERN.h>
#include <perl.h>

// ���� �� ���������� ��ü ����
static PerlInterpreter * my_perl;
int main(int argc, char** argv, char** env)
{
    //�� ��ũ��Ʈ �ý����� �ʱ�ȭ�ϰ� �� ���������� ��ü�� �����Ѵ�.
    PERL_SYS_INIT3(&argc, &argv, &env);
    my_perl = perl_alloc();
    perl_construct(my_perl);
    PL_exit_flags |= PERL_EXIT_DESTRUCT_END;
    // Add100.pl�� ��� ���ڷ� �Ѱܼ� ��ũ��Ʈ�� �Ľ��Ѵ�.
    perl_parse(my_perl, NULL, argc, argv, (char**)NULL);
    //��ũ��Ʈ�� �����Ѵ�.
    perl_run(my_perl);
    //�� ���������� ��ü�� �����Ѵ�.
    perl_destruct(my_perl);
    perl_free(my_perl);
    PERL_SYS_TERM();

    return 0;
}