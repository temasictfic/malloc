# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sciftci <sciftci@student.42kocaeli.com.tr> +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/06/21 07:52:51 by sciftci           #+#    #+#              #
#    Updated: 2022/07/06 00:36:20 by sciftci          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:= libft.a
CC		:= cc
CFLAGS	:= -Wall -Wextra -Werror
LIBC	:= ar -rcs
RM		:= rm -f
#$(shell find . -type f ! -name "ft_lst*.c" -name "ft_*.c")
SRCS			:=	ft_isalnum.c ft_isprint.c ft_memcmp.c  ft_putchar_fd.c ft_split.c \
					ft_strlcat.c ft_strncmp.c ft_substr.c ft_atoi.c ft_isalpha.c \
					ft_itoa.c ft_memcpy.c  ft_putendl_fd.c ft_strchr.c  ft_strlcpy.c \
					ft_strnstr.c ft_tolower.c ft_bzero.c   ft_isascii.c \
					ft_memmove.c ft_putnbr_fd.c  ft_strdup.c  ft_strlen.c  ft_strrchr.c \
					ft_toupper.c ft_calloc.c  ft_isdigit.c ft_memchr.c  ft_memset.c  \
					ft_putstr_fd.c  ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_striteri.c  \
					ft_putnchar_fd.c  ft_putstrn_fd.c  ft_count_digit_base.c  ft_uitoa.c


OBJS	:= $(SRCS:%.c=%.o)
INCS	:= libft.h
#$(wildcard ft_lst*.c)
BSRCS			:=	ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c \
					ft_lstdelone.c ft_lstiter.c ft_lstlast.c \
					ft_lstmap.c ft_lstnew.c ft_lstsize.c

B_OBJS	:= $(BSRCS:%.c=%.o)

#%.o: %.c
#	$(CC) $(CFLAGS) -c $< -o $(<:.c=.o) -I $(INCS)

all: $(NAME)

$(NAME): $(OBJS)
	$(LIBC) $(NAME) $(OBJS)

bonus: $(OBJS) $(B_OBJS)
		$(LIBC) $(NAME) $(OBJS) $(B_OBJS)

clean:
	$(RM) $(OBJS) $(B_OBJS)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all bonus clean fclean re