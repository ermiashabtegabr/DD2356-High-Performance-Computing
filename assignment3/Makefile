all:domain ping one_side

clean:clean_domain clean_ping clean_one

domain:
	cc domainDecom1D.c -o domainDecom1D
run_domain: domain
	srun -n 4 ./domainDecom1D
clean_domain: 
	rm domainDecom1D

ping:
	cc ping_pong.c -o ping_pong
run_ping_same:
	srun -n 2 ./ping_pong 
run_ping_diff:
	srun -n 2 --ntasks-per-node=1 ./ping_pong 
clean_ping:
	rm ping_pong output_different_nodes.txt output_same_node.txt

one_side:
	cc ping_pong_one_sided.c -o ping_pong_one_sided
run_diff:
	srun -n 2 --ntasks-per-node=1 ./ping_pong_one_sided
run_one:
	srun -n 2 ./ping_pong_one_sided
clean_one:
	rm ping_pong_one_side

pi_linear:
	cc pi_linear.c -o pi_linear
clean_linear:
	rm pi_linear

pi_binary:
	cc pi_binary.c -o pi_binary
clean_binary:
	rm pi_binary

linear_non:
	cc pi_linear_non_blocking.c -o pi_linear_non_blocking
clean_linear_non:
	rm pi_linear_non_blocking

