/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nidor <nidor@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 14:17:50 by nidor             #+#    #+#             */
/*   Updated: 2023/08/05 14:17:51 by nidor            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

void		print_request_header(int sock_clt, int sock_srv);
void		interruptResponse(int sock_clt, int sock_srv, const char *statusCode, const char *statusMessage);
void		serve_error_file(int sock_clt, int sock_srv);
int			response_part(int sock_clt, int sock_srv);
int		    deleteMethod(int sock_clt, int sock_srv);
int         postMethod(int sock_clt, int sock_srv);
std::string	getContentType(const char* path);
int			pathSecure(std::string path);
int         send_header(int sock_clt, int sock_srv, int size, const char *path);
int         proceedResponse(int sock_clt, int sock_srv);
int         getMethod(int sock_clt, int sock_srv);


extern std::map <std::string, std::string> response;

#endif
