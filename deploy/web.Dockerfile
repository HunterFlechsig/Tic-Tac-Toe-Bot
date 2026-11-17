FROM python:3.14-trixie AS backend

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        ninja-build \
        git \
    && rm -rf /var/lib/apt/lists/*

COPY --from=ghcr.io/astral-sh/uv:latest /uv /uvx /bin/
WORKDIR /src
COPY engine ./engine
COPY bindings ./bindings
COPY backend ./backend

WORKDIR /src/backend
RUN uv sync --frozen --no-dev --no-editable

FROM node:24-alpine AS frontend

WORKDIR /src
COPY frontend/package.json frontend/package-lock.json ./
RUN npm ci
COPY frontend ./
RUN npm run build

FROM python:3.14-slim-trixie

RUN apt-get update \
    && apt-get install -y --no-install-recommends \
        nginx \
        gettext-base \
    && rm -rf /var/lib/apt/lists/* \
    && rm -f /etc/nginx/sites-enabled/default

ENV PORT=80
ENV API_URL=http://127.0.0.1:5000

WORKDIR /src/backend
COPY --from=backend /src/backend /src/backend
COPY --from=frontend /src/dist /usr/share/nginx/html
COPY deploy/nginx.conf.template /etc/nginx/templates/default.conf.template
COPY deploy/web-entrypoint.sh /web-entrypoint.sh
RUN chmod +x /web-entrypoint.sh

EXPOSE 80
ENTRYPOINT ["/web-entrypoint.sh"]
